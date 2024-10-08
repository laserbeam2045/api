#include "BeamSearch.h"

// 周囲に展開する際の反復回数を返すマクロ（斜め移動の上限が0回ならば常に4を返す）
#define getMaxDirection(node, setting) (\
  node->movedCountDiagonally >= setting->diagonalLimit\
    ? DIRECTION_MIN\
    : DIRECTION_MAX\
)

// コンボ数を数える関数へのポインタ（盤面のサイズに応じて使い分ける）
static void (*BeamSearch_countCombo)(SearchNode*, SearchConditions*, bool) = NULL;

// プライベート関数
static void BeamSearch_initQueue(BeamSearch* this, BoardSettings *bsp);
static void BeamSearch_expandNodes(BeamSearch* this, SearchConditions *scp, int depth);
static bool BeamSearch_isMeaninglessMove(BeamSearch* this, SearchNode*, char currIndex, char nextIndex);
static void BeamSearch_mergeNodes(BeamSearch* this);
static void BeamSearch_cutBranch(BeamSearch* this);
static int cmp(const void*, const void*);
static int cmp2(const void*, const void*);
static void BeamSearch_swapQueues(BeamSearch* this);
static void BeamSearch_selectBestNode(BeamSearch* this, SearchConditions*);
static void BeamSearch_simulateDropFall(SearchNode* node, SearchConditions*);


// 初期化関数
void BeamSearch_init(BeamSearch* this, SearchConditions *searchConditions)
{
  BoardSettings *bsp = SearchConditions_getBoardSettings(searchConditions);
  SearchSettings *ssp = SearchConditions_getSearchSettings(searchConditions);

  this->endFlag = false;
  this->droppable = BoardSettings_getDroppable(bsp);
  this->beamWidth = SearchSettings_getBeamWidth(ssp);
  this->beamDepth = SearchSettings_getBeamDepth(ssp);
  this->maxThreads = omp_get_max_threads();

  int queueLength = this->beamWidth * this->maxThreads;  // １つのキューが保持できるノード数
  this->rootHashNode = malloc(sizeof(HashNode));
  this->threads = malloc(sizeof(Thread) * this->maxThreads);
  this->parents = malloc(sizeof(SearchNode) * queueLength);
  this->children = malloc(sizeof(SearchNode) * queueLength);
  this->parentsP = malloc(sizeof(int) * queueLength);
  this->childrenP = malloc(sizeof(int) * queueLength);

  // 二分木のルートを初期化
  uint64_t rootHashValue = 0;
  HashNode_init(this->rootHashNode, &rootHashValue);

  // スレッドオブジェクトを初期化
  for (int i = 0; i < this->maxThreads; i++) {
    int baseIndex = (queueLength / this->maxThreads) * i;
    Thread_init(&this->threads[i], baseIndex);
  }
  // 良質ノード集積オブジェクトを初期化
  ExcellentNodes_init(&this->excellentNodes);
  // 親となるキューを初期化
  BeamSearch_initQueue(this, bsp);

  // 使用する関数のポインタを決める
  switch (Board_length) {
    case 30: BeamSearch_countCombo = countCombo_5x6; break;
    case 42: BeamSearch_countCombo = countCombo_6x7; break;
  }
  // CountComboクラスの初期化
  CountCombo_init(bsp);
}


// 後始末関数
void BeamSearch_finish(BeamSearch* this)
{
  free(this->parents);
  free(this->children);
  free(this->parentsP);
  free(this->childrenP);
  free(this->threads);
  HashNode_finish(this->rootHashNode);
}


// ビームサーチを実行する関数
SearchNode BeamSearch_run(BeamSearch* this, SearchConditions *scp)
{
  // 指定された深さまでビーム探索を行う
  for (int depth = 0; depth < this->beamDepth; depth++) {
    if (this->endFlag) break;

    // キューからノードを取り出し、展開する
    BeamSearch_expandNodes(this, scp, depth);
    // スレッド別に、離れたアドレス上に持たせたノードを、連続するデータ（ポインタ配列）として統合する
    BeamSearch_mergeNodes(this);
    // ノード数がビーム幅に達していたら、評価値について降順に並べ、ビーム幅を次回の反復回数の上限とする
    BeamSearch_cutBranch(this);
    // 親のキューと子のキューを入れ替える（次のループでは子のキューを親として展開する）
    BeamSearch_swapQueues(this);
  }
  // 落ちコンありの設定なら、落ちコンをシミュレートした上で最良ノードを決める
  if (this->droppable) {
    BeamSearch_selectBestNode(this, scp);
  }
  // 最良ノードを返す
  return ExcellentNodes_getBestNode(&this->excellentNodes);
}


// キューを初期化する関数
// *bsp   盤面に関する設定のポインタ
static void BeamSearch_initQueue(BeamSearch* this, BoardSettings *bsp)
{
  Board *board = BoardSettings_getBoard(bsp);

  this->parentsCount = 0;
  // 盤面のドロップのそれぞれについてキューに追加する
  for (char position = 0; position < Board_length; position++) {
    SearchNode *searchNode = &this->parents[this->parentsCount];

    // 開始位置指定があり、かつ異なる座標の場合はキューに入れない
    if (BoardSettings_isUnstartable(bsp, position)) continue;
    // 操作不可地点の場合はキューに入れない
    if (BoardSettings_isNoEntryPosition(bsp, position)) continue;
    // ビームサーチノードを初期化する
    SearchNode_init(searchNode, board, position);
    // 局面のハッシュ値を二分木に登録する
    HashNode_makeTree(this->rootHashNode, SearchNode_getHashValue(searchNode));

    // ポインタ版のキューには探索ノードのアドレスを入れる
    // MEMO: 構造体のコピーを最小限にするためにアドレスを使用する
    this->parentsP[this->parentsCount] = searchNode;
    this->parentsCount++;
  }
}


// キューからノードを取り出し、展開する関数
// moveCost 深さに応じた移動コスト
// *scp     探索条件オブジェクトのアドレス
static void BeamSearch_expandNodes(BeamSearch* this, SearchConditions *scp, int depth)
{
  BoardSettings *bsp = SearchConditions_getBoardSettings(scp);
  SearchSettings *ssp = SearchConditions_getSearchSettings(scp);
  ExcellentNodes *enp = &this->excellentNodes;
  SearchNode *parentNode = NULL;
  SearchNode *childNode = NULL;
  ComboData *comboData = NULL;
  uint64_t *hashValue = NULL;
  Thread *thread = NULL;
  char prevIndex, currIndex, nextIndex, maxDirection;
  int i, j, threadId, childIndex;
  double evaluation;

  // スレッドごとの子ノード数を0で初期化する
  for (i = 0; i < this->maxThreads; i++) {
    Thread_setNodeCount(&this->threads[i], 0);
  }

  // i(親ノード)のループについて並列化する指示文
  #pragma omp parallel for num_threads(this->maxThreads)\
          private(parentNode, childNode, comboData, hashValue, prevIndex, currIndex,\
                  nextIndex, maxDirection, j, threadId, thread, childIndex, evaluation)
  for (i = 0; i < this->parentsCount; i++) {
    parentNode = this->parentsP[i];                       // 親ノード
    prevIndex = SearchNode_getPreviousIndex(parentNode);  // 直前の座標
    currIndex = SearchNode_getCurrentIndex(parentNode);   // 現在の座標
    maxDirection = getMaxDirection(parentNode, ssp);      // 展開する方向の数
    threadId = omp_get_thread_num();                      // スレッドIDを取得
    thread = &this->threads[threadId];                    // スレッドオブジェクト

    // 上下左右４方向（または８方向）へ展開する
    for (j = 0; j < maxDirection; j++) {
      nextIndex = getNextIndex(currIndex, j);   // 移動先の座標
      if (nextIndex < 0) continue;              // マイナスの座標は展開できない方向を意味する
      if (nextIndex == prevIndex) continue;     // 直前の座標に戻るのは無駄なので省く

      // 斜め移動の場合、非効率な移動なら展開しない
      if (3 < j && BeamSearch_isMeaninglessMove(this, parentNode, currIndex, nextIndex)) continue;

      // 次の座標が操作不可地点なら展開しない
      if (BoardSettings_isNoEntryPosition(bsp, nextIndex)) continue;

      // スレッドに応じて、次に使用する配列のインデックスを算出
      childIndex = Thread_getNextIndex(thread);
      childNode = &this->children[childIndex];

      // 親ノードのデータを子ノードにコピーする
      SearchNode_copyWithoutComboData(childNode, parentNode);

      // 子ノードを移動させる
      hashValue = SearchNode_moveTo(childNode, nextIndex, j);

      // 調査済みの局面なら展開しない
      if (!HashNode_makeTree(this->rootHashNode, hashValue)) continue;

      // 子ノード数を加算する（ここで初めて展開が確定）
      Thread_incrementNodeCount(thread);

      // コンボ情報を初期化して解析し、評価関数に渡す
      SearchNode_initComboData(childNode);
      BeamSearch_countCombo(childNode, scp, false);
      comboData = SearchNode_getComboData(childNode);
      evaluation = evaluate(comboData, scp, depth);

      // 最高評価値を更新したら、優秀なノードとしてスタックに追加する
      if (ExcellentNodes_getBestEvaluation(enp) * 0.999 <= evaluation) {
        ExcellentNodes_setBestNode(enp, childNode);
        // printf("%10.10lf\n", evaluation);
        // 条件を満たせば探索を終了するフラグを立てる
        if (SearchConditions_isEnoughAchievement(scp, comboData)) {
          this->endFlag = true;
          break;
        }
      }
    }
  }
}


// 斜め移動が効果的かどうかを判別する関数
// 戻り値：true(非効率) or false(効果的)
static bool BeamSearch_isMeaninglessMove(BeamSearch* this, SearchNode *searchNode,
                                                      char currIndex, char nextIndex)
{
  Board *board = SearchNode_getBoard(searchNode);
  char nextColor = Board_getColor(board, nextIndex);
  char cornerColor1, cornerColor2;

  if (  // 左上または左下に移動しようとしている場合
    (currIndex - nextIndex == Board_width + 1) ||
    (currIndex - nextIndex == -Board_width + 1)
  ) {
    cornerColor1 = Board_getColor(board, currIndex - 1);
    cornerColor2 = Board_getColor(board, nextIndex + 1);
  }
  else if ( // 右上または右下に移動しようとしている場合
    (currIndex - nextIndex == Board_width - 1) ||
    (currIndex - nextIndex == -Board_width - 1)
  ) {
    cornerColor1 = Board_getColor(board, currIndex + 1);
    cornerColor2 = Board_getColor(board, nextIndex - 1);
  }
  if (
    (nextColor == cornerColor1) ||
    (nextColor == cornerColor2) ||
    (cornerColor1 == cornerColor2)
  ) {
    return true;
  } else {
    return false;
  }
}


// スレッド別に、離れたアドレス上に持たせたノードを、
// 連続するデータ（ポインタ配列）として統合する関数
static void BeamSearch_mergeNodes(BeamSearch* this)
{
  this->childrenCount = 0;
  for (int threadId = 0; threadId < this->maxThreads; threadId++) {
    Thread thread = this->threads[threadId];
    int baseIndex = Thread_getBaseIndex(&thread);
    int nodeCount = Thread_getNodeCount(&thread);
    for (int i = 0; i < nodeCount; i++) {
      this->childrenP[this->childrenCount] = &this->children[baseIndex + i];
      this->childrenCount++;
    }
  }
}


// ノード数がビーム幅に達している場合、評価値について降順に並べ替える関数
static void BeamSearch_cutBranch(BeamSearch* this)
{
  if (this->beamWidth <= this->childrenCount) {
    qsort(this->childrenP, this->childrenCount, sizeof(this->childrenP[0]), cmp);
    this->parentsCount = this->beamWidth;
  } else {
    this->parentsCount = this->childrenCount;
  }
}


// ビームサーチノードのソート条件（評価値について降順に並べる）
static int cmp(const void *a, const void *b)
{
  double difference = (*(SearchNode**)b)->comboData.evaluation
                    - (*(SearchNode**)a)->comboData.evaluation;

  if (0 == difference) return 0;
  if (0 < difference)  return 1;
  return -1;
}


// ビームサーチノードのソート条件（評価値について降順に並べる）
static int cmp2(const void *a, const void *b)
{
  double difference = (*(SearchNode*)b).comboData.evaluationProbability
                    - (*(SearchNode*)a).comboData.evaluationProbability;

  if (0 == difference) return 0;
  if (0 < difference)  return 1;
  return -1;
}


// 親のキューと子のキューを入れ替える関数
static void BeamSearch_swapQueues(BeamSearch* this)
{
  SearchNode *temp1 = NULL;
  SearchNode **temp2 = NULL;

  temp1 = this->parents;
  this->parents = this->children;
  this->children = temp1;

  temp2 = this->parentsP;
  this->parentsP = this->childrenP;
  this->childrenP = temp2;
}


// 優れたノードのそれぞれについて、落ちコンをシミュレートして最良ノードを探す関数
// searchConditions   探索に関する設定
static void BeamSearch_selectBestNode(BeamSearch* this, SearchConditions *searchConditions)
{
  ExcellentNodes *enp = &this->excellentNodes;
  int nodeCount = ExcellentNodes_getNodeCount(enp);     // スタックされたノード数
  int targetNodeNum = fmin(nodeCount, STACK_NODE_MAX);  // 試行対象となるノードの数
  SearchNode *searchNode = NULL;
  ComboData *comboData = NULL;

  #pragma omp parallel for private(searchNode, comboData) num_threads(this->maxThreads)
  for (int i = 0; i < targetNodeNum; i++) {
    searchNode = ExcellentNodes_getNodeOf(enp, i);
    comboData = SearchNode_getComboData(searchNode);

    BeamSearch_simulateDropFall(searchNode, searchConditions);

    #pragma omp critical  // 一度に実行できるスレッドを１つに制限
    ExcellentNodes_setNode(enp, searchNode, i);
  }

  ExcellentNodes_sortNodes(enp, &cmp2);

  for (int i = 0; i < 10; i++) {
    searchNode = ExcellentNodes_getNodeOf(enp, i);
    comboData = SearchNode_getComboData(searchNode);
    double evaluation1 = ComboData_getEvaluation(comboData);
    double evaluation2 = ComboData_getEvaluationProbability(comboData);

    // printf("\n%3d %8.0lf\n%12.0lf\n", i + 1, evaluation1, evaluation2);
  }
}


// 落ちコンをシミュレートして平均評価値を求める関数
// *searchNode        対象となるノードを指すアドレス
// *searchConditions  探索に関する設定
static void BeamSearch_simulateDropFall(SearchNode *searchNode, SearchConditions *searchConditions)
{
  double total[3] = {0};              // コンボ数・倍率・評価値のそれぞれの合計値
  double evaluations[SIMULATION_MAX]; // 評価値のそれぞれの値
  double normalizedEvaluations[SIMULATION_MAX]; // 正則化された評価値のそれぞれの値
  double stableValue = 1;
  char movedCount = SearchNode_getMovedCount(searchNode);

  ComboData *comboData = SearchNode_getComboData(searchNode);

  // コンボ情報を初期化して解析し、評価値を得ることを繰り返す
  for (int i = 0; i < SIMULATION_MAX; i++) {
    SearchNode_initComboData(searchNode);
    BeamSearch_countCombo(searchNode, searchConditions, true);
    evaluate(comboData, searchConditions, movedCount);

    evaluations[i] = ComboData_getEvaluation(comboData);

    total[0] += ComboData_getCombo(comboData);
    total[1] += ComboData_getMagnification(comboData);
    total[2] += evaluations[i];
  }

  // 基本的なコンボ情報は、落ちコンなしで調べた状態に戻す
  SearchNode_initComboData(searchNode);
  BeamSearch_countCombo(searchNode, searchConditions, false);

  double comboValue = total[0] / SIMULATION_MAX;
  double magnification = total[1] / SIMULATION_MAX;
  double evaluationValue = total[2] / SIMULATION_MAX;

  // 2種類の属性について、最終的な平均値を記録する
  ComboData_setCombo(comboData, comboValue);
  ComboData_setMagnification(comboData, magnification);

  // 評価値は正則化して、安定したものに付加価値を与える
  for (int i = 0; i < SIMULATION_MAX; i++) {
    normalizedEvaluations[i] = 1 + (evaluations[i] / total[2]);
  }

  // 正則化した評価値を全てかけ合わせる
  for (int i = 0; i < SIMULATION_MAX; i++) {
    stableValue *= normalizedEvaluations[i];
  }

  // printf("%5.10lf\n", stableValue);

  // printf("%5.10lf\n", fabs(M_E - stableValue));

  // ネイピア数との差が小さいほど安定している
  stableValue = pow(fabs(1 - fabs(M_E - stableValue)), 5000);

  // printf("%5.10lf\n\n", stableValue);

  ComboData_setEvaluation(comboData, evaluationValue);
  ComboData_setEvaluationProbability(comboData, evaluationValue * stableValue);
}