#ifndef _ADJACENT_H_
#define _ADJACENT_H_

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../constants.h"

/*
  隣接点クラス
*/

// 次（indexから見たdirection方向）の座標を取得するマクロ
#define getNextIndex(index, direction) (Adjacent_table[(index << 3) + direction])

// 隣接点テーブルポインタ
extern char *Adjacent_table;

// 初期化関数（最初に一度だけ呼ぶ必要がある）
extern void Adjacent_init(void);

// 盤面のサイズに応じて使用するテーブルを切り替える関数
extern void Adjacent_initTablePointer(char boardLength);

#endif  // _ADJACENT_H_