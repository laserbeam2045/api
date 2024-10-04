#include "count_combo.h"

// 落ちうる色かどうかを判定するマクロ
#define isDroppable(color) (CountCombo_droppable & (1 << color))
// 消せる色かどうかを判定するマクロ
#define isClearable(color) (CountCombo_clearable & (1 << color))

// プライベート定数
// MEMO:6x7版との区別のために、staticでスコープを分けている
static const char BOARD_HEIGHT = 5;                   // 盤面の高さ
static const char BOARD_WIDTH = 6;                    // 盤面の横幅
static const char BOARD_WIDTH_DOUBLE = 12;            // 盤面の横幅 × 2
static const char BOARD_LENGTH = 30;                  // 盤面のドロップ数
static const char ITERATE_HORIZONTAL_MAX = 4;         // 横に探索する回数（BOARD_WIDTH - 2）
static const char ITERATE_VERTICAL_MAX = 3;           // 縦に探索する回数（BOARD_HEIGHT - 2）
static const char CLEAR_START_INDEX = 2;              // ドロップを消し始める座標（(1 - 1) + 2）
static const char CLEAR_END_INDEX = 27;               // ドロップを消し終える座標（(BOARD_LENGTH - 1) - 2）
static const char BOTTOM_LEFT_INDEX = 24;             // 盤面の左下の座標
static const int BITCOMBO_HORIZONTAL = 0b111;         // 横にコンボする際に論理和を適用する値
static const int BITCOMBO_VERTICAL = 0b1000001000001; // 縦にコンボする際に論理和を適用する値

// プライベート関数
// MEMO:6x7版との区別のために、staticでスコープを分けている
static int markClearablePlace(Board *board);
static bool isExplosion(Board *board, int bitCombo, ComboData*, BoardSettings*);
static void explode(Board *board, int bitCombo, char bombX, char bombY, ComboData*);
static void clearDrops(Board *board, int, ComboData*, ClearingSettings*, LeaderSettings*);
static void _clearDrops(Board *board, char, char, int, int*, char*, char[]);
static void dropDrops(Board *board);
static void fillSpace(Board *board);


// コンボ数などを数える関数
void countCombo_5x6(SearchNode *searchNode, SearchConditions *searchConditions, bool dropFallFlag)
{
  ComboData *comboData = SearchNode_getComboData(searchNode);
  BoardSettings *bsp = SearchConditions_getBoardSettings(searchConditions);
  LeaderSettings *lsp = SearchConditions_getLeaderSettings(searchConditions);
  ClearingSettings *csp = SearchConditions_getClearingSettings(searchConditions);
  Board board;

  // 盤面をコピーして、新しいBoardインスタンスを使う
  Board_init(&board, SearchNode_getBoardState(searchNode));

  while (1) {
    // ドロップが消える位置をビットに記録する
    int bitCombo = markClearablePlace(&board);

    // 盤面に爆弾が存在する場合、爆発するかどうかを確認する
    if (ComboData_getLeftovers(comboData, (DROP_TYPE)BOMB)) {
      // 爆発する場合は消える位置を記録しなおす
      if (isExplosion(&board, bitCombo, comboData, bsp)) {
        continue;
      }
    }

    // 消えるドロップがなければ終了
    if (!bitCombo) return;

    // ドロップを消す
    clearDrops(&board, bitCombo, comboData, csp, lsp);

    // ドロップを落とす
    dropDrops(&board);

    // 落ちコンありの指定ならば空いたスペースを埋める
    if (dropFallFlag) {
      fillSpace(&board);
    }
  }
}


/* ドロップが消える位置を探し、ビットに記録する関数
char board[]     対象の盤面
戻り値：消える位置を記録した値
*/
static int markClearablePlace(Board *board)
{
  char X, Y, index, color;
  int bitCombo = 0;

  // 横の探索
  for (Y = 0; Y < BOARD_HEIGHT; Y++) {
    for (X = 0; X < ITERATE_HORIZONTAL_MAX; X++) {
      index = BOARD_WIDTH * Y + X;
      color = Board_getColor(board, index);
      if (
        isClearable(color) &&
        color != (DROP_TYPE)NONE &&
        color == Board_getColor(board, index + 1) &&
        color == Board_getColor(board, index + 2)
      ) {
        bitCombo |= BITCOMBO_HORIZONTAL << index;
      }
    }
  }
  // 縦の探索
  for (X = 0; X < BOARD_WIDTH; X++) {
    for (Y = 0; Y < ITERATE_VERTICAL_MAX; Y++) {
      index = BOARD_WIDTH * Y + X;
      color = Board_getColor(board, index);
      if (
        isClearable(color) &&
        color != (DROP_TYPE)NONE &&
        color == Board_getColor(board, index + BOARD_WIDTH) &&
        color == Board_getColor(board, index + BOARD_WIDTH_DOUBLE)
      ) {
        bitCombo |= BITCOMBO_VERTICAL << index;
      }
    }
  }
  return bitCombo;
}

// 爆弾が爆発するかどうかを確認する関数
static bool isExplosion(Board *board, int bitCombo, ComboData* comboData, BoardSettings* boardSettings)
{
  bool flag = false;

  for (char Y = 0; Y < BOARD_HEIGHT; Y++) {
    for (char X = 0; X < BOARD_WIDTH; X++) {
      char index = BOARD_WIDTH * Y + X;
      char color = Board_getColor(board, index);

      if (color == (DROP_TYPE)BOMB && !(bitCombo & (1 << index))) {
        char explosionCount = ComboData_getExplosionCount(comboData);

        // 操作不可の位置にある爆弾が爆発する場合はペナルティが重くなるようにする
        if (BoardSettings_isNoEntryPosition(boardSettings, index)) {
          ComboData_setExplosionCount(comboData, explosionCount + 10);
        } else {
          ComboData_setExplosionCount(comboData, explosionCount + 1);
        }
        explode(board, bitCombo, X, Y, comboData);
        flag = true;
      }
    }
  }
  return flag;
}

// 爆弾の縦横に存在するドロップを消す関数
static void explode(Board *board, int bitCombo, char bombX, char bombY, ComboData* comboData)
{
  char X, Y, index, color;

  for (Y = 0; Y < BOARD_HEIGHT; Y++) {
    index = BOARD_WIDTH * Y + bombX;
    color = Board_getColor(board, index);
    if (color != (DROP_TYPE)NONE && color != (DROP_TYPE)BOMB) {
      Board_setColor(board, index, (DROP_TYPE)NONE);
      ComboData_decreaseLeftovers(comboData, color, 1);
    }
  }
  for (X = 0; X < BOARD_WIDTH; X++) {
    index = BOARD_WIDTH * bombY + X;
    color = Board_getColor(board, index);
    if (color != (DROP_TYPE)NONE && color != (DROP_TYPE)BOMB) {
      Board_setColor(board, index, (DROP_TYPE)NONE);
      ComboData_decreaseLeftovers(comboData, color, 1);
    }
  }
  index = BOARD_WIDTH * bombY + bombX;
  Board_setColor(board, index, (DROP_TYPE)NONE);
  ComboData_decreaseLeftovers(comboData, (DROP_TYPE)BOMB, 1);
}


/* ドロップを消す関数を呼び、情報を記録する関数
char board[]    対象の盤面
int bitCombo    消える位置が記録された変数
ComboData *data 情報を記録するノード
clearingSettings *clearingSettings  消し方に関する設定
*/
static void clearDrops(Board *board, int bitCombo, ComboData *comboData, ClearingSettings *csp, LeaderSettings *lsp)
{
  int checked = 0;

  ComboData_incrementStep(comboData);
  for (char index = CLEAR_START_INDEX; index <= CLEAR_END_INDEX; index++) {
    // ドロップが消える座標ではない、または、確認済みの座標であるときは、飛ばす
    if ( !(bitCombo & (1 << index)) || (checked & (1 << index)) ) {
      continue;
    }
    char clearCount = 0;
    char clearIndices[BOARD_LENGTH];
    char color = Board_getColor(board, index);

    // ドロップを消し、消える数と座標を取得する
    _clearDrops(board, index, color, bitCombo, &checked, &clearCount, clearIndices);
    // コンボ情報を記録する
    ComboData_markCombo(comboData, color, clearCount);

    switch (clearCount) {
    case 3:
      break;
    case 4:
      if (ClearingSettings_isActiveOf(csp, (CS_TYPE)FOUR, color)) {
        ComboData_incrementClearStyle(comboData, (CS_TYPE)FOUR, color);
      }
      break;
    case 5:
      if (
        ClearingSettings_isActiveOf(csp, (CS_TYPE)LINE, 7) &&
        (DROP_TYPE)HEART == color &&
        isVLine(clearIndices, clearCount)
      ) {
        ComboData_incrementClearStyle(comboData, (CS_TYPE)LINE, 7);
      }
      else if (
        isL(clearIndices, clearCount)
      ) {
        ComboData_incrementClearStyle(comboData, (CS_TYPE)L, color);
      }
      else if (
        (
          LeaderSettings_isCrossLeader(lsp) ||
          ClearingSettings_isActiveOf(csp, (CS_TYPE)CROSS, color)
        )
        && isCross(clearIndices, clearCount)
      ) {
        ComboData_incrementClearStyle(comboData, (CS_TYPE)CROSS, color);
      }
      break;
    default:
      if (
        ClearingSettings_isActiveOf(csp, (CS_TYPE)LINE, color) &&
        isHLine(clearIndices, clearCount)
      ) {
        ComboData_incrementClearStyle(comboData, (CS_TYPE)LINE, color);
      }
      else if (
        ClearingSettings_isActiveOf(csp, (CS_TYPE)SQUARE, color) &&
        is3x3(clearIndices, clearCount)
      ) {
        ComboData_incrementClearStyle(comboData, (CS_TYPE)SQUARE, color);
      }
      if (
        10 <= clearCount && clearCount <= 12 &&
        ClearingSettings_isActiveOf(csp, (CS_TYPE)LINE, color) &&
        ComboData_getAdditionalCombo(comboData) == 0
      ) {
        ComboData_setAdditionalCombo(comboData, 2);
      }
    }
  }
}


/* ドロップを消す関数
char board[]        盤面を表す配列
char index          対象となる座標
char color          ドロップの色
int  bitCombo       消える座標をビットに記録した値
int  *checked       調査済みの座標をビットに記録した値
char *clearCount    消したドロップの数
char clearIndices[] ドロップを消した座標の配列
*/
static void _clearDrops(Board *board, char index, char color, int bitCombo,
                        int *checked, char *clearCount, char clearIndices[])
{
  Board_setColor(board, index, (DROP_TYPE)NONE);
  *checked |= 1 << index;
  clearIndices[*clearCount] = index;
  (*clearCount)++;

  for (char direction = 0; direction < DIRECTION_MIN; direction++) {
    char nextIndex = getNextIndex(index, direction);

    if (nextIndex < 0) {
      return;
    }
    else if (
       (bitCombo & (1 << nextIndex)) &&
      !(*checked & (1 << nextIndex)) &&
      Board_getColor(board, nextIndex) == color
    ) {
      _clearDrops(board, nextIndex, color, bitCombo, checked, clearCount, clearIndices);
    }
  }
}


/* ドロップを落とす関数
char board[] 対象の盤面
*/
static void dropDrops(Board *board)
{
  for (char index = BOTTOM_LEFT_INDEX; index--;) {
    if ((DROP_TYPE)NONE != Board_getColor(board, index)) {
      char lowerIndex = index;

      while (
        lowerIndex < BOTTOM_LEFT_INDEX &&
        (DROP_TYPE)NONE == Board_getColor(board, lowerIndex + BOARD_WIDTH)
      ) {
        lowerIndex += BOARD_WIDTH;
      }
      if (index != lowerIndex) {
        Board_swapColor(board, index, lowerIndex);
      }
    }
  }
}


/* ドロップが落ちて空いたスペースにドロップを埋める関数
char board[]      対象の盤面
*/
static void fillSpace(Board *board)
{
  char color;
  for (char index = BOARD_LENGTH; index--;) {
    if ((DROP_TYPE)NONE == Board_getColor(board, index)) {
      do {
        color = rand() % DROP_TYPE_MAX + 1;
      } while (!isDroppable(color));
      Board_setColor(board, index, color);
    }
  }
}