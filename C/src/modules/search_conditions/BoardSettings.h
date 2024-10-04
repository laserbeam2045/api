#ifndef _BOARD_SETTINGS_H_
#define _BOARD_SETTINGS_H_

#include <stdbool.h>
#include "../../constants.h"
#include "../io/Parser.h"
#include "../Board.h"

// 盤面に関する設定
typedef struct {
  Board board;                          // 初期盤面
  int clearable;                        // 消せるドロップ（ビットフラグ）
  int droppable;                        // 落ちうるドロップ（ビットフラグ）
  char startPosition;                   // 開始位置指定
  char noEntryPositions[BOARD_LEN_MAX]; // 操作不可地点
  char noEntryPositionsCount;           // 操作不可地点の数
} BoardSettings;

// 初期化関数
extern void BoardSettings_init(BoardSettings* this, char request[]);

// board属性の先頭ポインタを返す関数
extern Board* BoardSettings_getBoard(BoardSettings* this);

// clearable属性を返す関数
extern int BoardSettings_getClearable(BoardSettings* this);

// droppable属性を返す関数
extern int BoardSettings_getDroppable(BoardSettings* this);

// 与えられた引数の座標が、開始位置として選択可能かどうかを判定する関数
// position    判定対象となる座標
// 戻り値：true（選択不可能）or false（ 選択可能）
extern bool BoardSettings_isUnstartable(BoardSettings* this, const char position);

// 与えられた引数の座標が、操作不可地点に含まれるかどうかを判定する関数
// position    判定対象となる座標
// 戻り値：true（含まれている）or false（ 含まれていない）
extern bool BoardSettings_isNoEntryPosition(BoardSettings* this, const char position);

#endif  // _BOARD_SETTINGS_H_