#include "count_combo.h"

// 落ちうる色かどうかを判定するマクロ
#define isDroppable(color) (CountCombo_droppable & (UINT64_1 << color))
// 消せる色かどうかを判定するマクロ
#define isClearable(color) (CountCombo_clearable & (UINT64_1 << color))

// プライベート定数
// MEMO:6x7版との区別のために、staticでスコープを分けている
static const char BOARD_HEIGHT = 6;             // 盤面の高さ
static const char BOARD_WIDTH = 7;              // 盤面の横幅
static const char BOARD_WIDTH_DOUBLE = 14;      // 盤面の横幅 × 2
static const char BOARD_LENGTH = 42;            // 盤面のドロップ数
static const char ITERATE_HORIZONTAL_MAX = 5;   // 横に探索する回数（BOARD_WIDTH - 2）
static const char ITERATE_VERTICAL_MAX = 4;     // 縦に探索する回数（BOARD_HEIGHT - 2）
static const char CLEAR_START_INDEX = 2;        // ドロップを消し始める座標（(1 - 1) + 2）
static const char CLEAR_END_INDEX = 39;         // ドロップを消し終える座標（(BOARD_LENGTH - 1) - 2）
static const char BOTTOM_LEFT_INDEX = 35;       // 盤面の左下の座標
static const uint64_t BITCOMBO_HORIZONTAL = 0b111;            // 横にコンボする際に論理和を適用する値
static const uint64_t BITCOMBO_VERTICAL = 0b100000010000001;  // 縦にコンボする際に論理和を適用する値
static const uint64_t UINT64_1 = 1;                           // 64ビット定数の1（キャスト忘れ防止のため）

// プライベート関数
// MEMO:5x6版との区別のために、staticでスコープを分けている
static uint64_t markClearablePlace(Board *board);
static void clearDrops(Board *board, uint64_t, ComboData*, ClearingSettings*, LeaderSettings*);
static void _clearDrops(Board *board, char, char, uint64_t, uint64_t*, char*, char[]);
static void dropDrops(Board *board);
static void fillSpace(Board *board);


// コンボ数などを数える関数
void countCombo_6x7(SearchNode *searchNode, SearchConditions *searchConditions, bool dropFallFlag)
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
    uint64_t bitCombo = markClearablePlace(&board);

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
static uint64_t markClearablePlace(Board *board)
{
  char X, Y, index, color;
  uint64_t bitCombo = 0;

  // 横の探索
  for (Y = 0; Y < BOARD_HEIGHT; Y++) {
    for (X = 0; X < ITERATE_HORIZONTAL_MAX; X++) {
      index = BOARD_WIDTH * Y + X;
      color = Board_getColor(board, index);
      if (
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


/* ドロップを消す関数を呼び、情報を記録する関数
char board[]      対象の盤面
uint64_t bitCombo 消える位置が記録された変数
ComboData *data   情報を記録するノード
clearingSettings *clearingSettings  消し方に関する設定
*/
static void clearDrops(Board *board, uint64_t bitCombo, ComboData *comboData, ClearingSettings *csp, LeaderSettings *lsp)
{
  uint64_t checked = 0;

  ComboData_incrementStep(comboData);
  for (char index = CLEAR_START_INDEX; index <= CLEAR_END_INDEX; index++) {
    // ドロップが消える座標ではない、または、確認済みの座標であるときは、飛ばす
    if ( !(bitCombo & (UINT64_1 << index)) || (checked & (UINT64_1 << index)) ) {
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
    case 6:
      if (
        ClearingSettings_isActiveOf(csp, (CS_TYPE)LINE, 7) &&
        (DROP_TYPE)HEART == color &&
        isVLine(clearIndices, clearCount)
      ) {
        ComboData_incrementClearStyle(comboData, (CS_TYPE)LINE, 7);
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
uint64_t bitCombo   消える座標をビットに記録した値
uint64_t *checked   調査済みの座標をビットに記録した値
char *clearCount    消したドロップの数
char clearIndices[] ドロップを消した座標の配列
*/
static void _clearDrops(Board *board, char index, char color, uint64_t bitCombo,
                        uint64_t *checked, char *clearCount, char clearIndices[])
{
  Board_setColor(board, index, (DROP_TYPE)NONE);
  *checked |= UINT64_1 << index;
  clearIndices[*clearCount] = index;
  (*clearCount)++;

  for (char direction = 0; direction < DIRECTION_MIN; direction++) {
    char nextIndex = getNextIndex(index, direction);

    if (nextIndex < 0) {
      return;
    }
    else if (
       (bitCombo & (UINT64_1 << nextIndex)) &&
      !(*checked & (UINT64_1 << nextIndex)) &&
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
int  droppable  落ちうる色（ビットフラグ）
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