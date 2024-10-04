#ifndef _ZOBRIST_H_
#define _ZOBRIST_H_

#include "../../SFMT/SFMT.h"
#include "../constants.h"
#include "Board.h"

/*
  ゾブリストハッシュ法で使用する、ハッシュ値を扱うクラス
*/

// 乱数テーブルを初期化する関数（最初に一度だけ呼ぶ必要がある）
extern void ZobristHash_init(void);

// 与えられた局面に対応するハッシュ値を取得する関数
// *board     盤面オブジェクトのアドレス
// position   最初に選択するドロップの座標
extern uint64_t ZobristHash_getHashValue(Board *board, char position);

// 引数のハッシュ値を、ドロップを入れ替えた状態へと更新して返す関数
// hashValue  現在のハッシュ値
// index1     現在つかんでいるドロップの座標
// index2     移動先のドロップの座標
// color1     現在つかんでいるドロップの色
// color2     移動先のドロップの色
extern uint64_t ZobristHash_getSwappedHashValue(uint64_t hashValue, char index1, char index2, char color1, char color2);

#endif  // _ZOBRIST_H_