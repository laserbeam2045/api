#ifndef _BOARD_H_
#define _BOARD_H_

#include "../constants.h"

// 盤面クラス
typedef struct {
  char state[BOARD_LEN_MAX];
} Board;

// 盤面のサイズに関するクラス属性を初期化する関数（最初に一度だけ呼ぶ必要がある）
// height   盤面の高さ
// width    盤面の横幅
extern void Board_initClass(Board* this, const char height, const char width);

// 盤面の状態に関する初期化関数（インスタンスごとに呼ぶ）
// state[]  盤面の状態を保持する配列
extern void Board_init(Board* this, const char state[]);

// state属性の指定されたインデックスに、指定された値をセットする関数
// index    対象インデックス
// color    セットする値
extern void Board_setColor(Board* this, const char index, const char color);

// state属性の指定されたインデックスの値を返す関数
// index    対象インデックス
extern char Board_getColor(Board* this, const char index);

// 指定されたインデックスの要素同士を入れ替える関数
// index1   対象インデックス
// index2   〃
extern void Board_swapColor(Board* this, const char index1, const char index2);

// state属性の先頭アドレスを取得する関数
extern const char* Board_getState(Board* this);

// クラス属性
extern char Board_height;
extern char Board_width;
extern char Board_length;

#endif  // _BOARD_H_