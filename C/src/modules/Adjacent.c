#include "Adjacent.h"


// 隣接点テーブルポインタ
// MEMO: Adjacent_table_5x6か、Adjacent_table_6x7のいずれかを指す
char *Adjacent_table = NULL;

// プライベート変数
static char Adjacent_table_5x6[DIRECTION_MAX * (5 * 6)];
static char Adjacent_table_6x7[DIRECTION_MAX * (6 * 7)];

// プライベート関数
static void Adjacent_initTable(char table[], const char height, const char width);


// テーブルを初期化する関数（最初に一度だけ呼ぶ必要がある）
void Adjacent_init(void)
{
  Adjacent_initTable(Adjacent_table_5x6, 5, 6);
  Adjacent_initTable(Adjacent_table_6x7, 6, 7);
}

// 盤面のサイズに応じて使用するテーブルを切り替える関数
void Adjacent_initTablePointer(char boardLength)
{
  switch(boardLength) {
  case 30: Adjacent_table = Adjacent_table_5x6; break;
  case 42: Adjacent_table = Adjacent_table_6x7; break;
  }
}

// 隣接点テーブルを初期化する関数
// char table[]  データを入れる配列
// char height   盤面の高さ
// char width    盤面の横幅
static void Adjacent_initTable(char table[], const char height, const char width)
{
  for (char index = 0; index < height * width; index++) {
    char *buf0 = &table[DIRECTION_MAX * index],
         X = (index % width) + 1,
         Y = floor(index / width) + 1,
         N  = index - width,  // indexから見て上の座標
         S  = index + width,  // indexから見て下の座標
         W  = index - 1,      // indexから見て左の座標
         E  = index + 1,      // indexから見て右の座標
         NE = N + 1,          // indexから見て右上の座標
         NW = N - 1,          // indexから見て左上の座標
         SE = S + 1,          // indexから見て右下の座標
         SW = S - 1;          // indexから見て左下の座標

    if (1 == X && 1 == Y) {
      char topLeft[] = {E, S, -1, -1, SE, -2, -2, -2};
      memcpy(buf0, topLeft, DIRECTION_MAX);
    }
    else if (width == X && 1 == Y) {
      char topRight[] = {W, S, -1, -1, SW, -2, -2, -2};
      memcpy(buf0, topRight, DIRECTION_MAX);
    }
    else if (1 == X && height == Y) {
      char bottomLeft[] = {N, E, -1, -1, NE, -2, -2, -2};
      memcpy(buf0, bottomLeft, DIRECTION_MAX);
    }
    else if (width == X && height == Y) {
      char bottomRight[] = {N, W, -1, -1, NW, -2, -2, -2};
      memcpy(buf0, bottomRight, DIRECTION_MAX);
    }
    else if (1 == Y) {
      char top[] = {W, E, S, -1, SW, SE, -2, -2};
      memcpy(buf0, top, DIRECTION_MAX);
    }
    else if (1 == X) {
      char left[] = {N, E, S, -1, NE, SE, -2, -2};
      memcpy(buf0, left, DIRECTION_MAX);
    }
    else if (width == X) {
      char right[] = {N, W, S, -1, NW, SW, -2, -2};
      memcpy(buf0, right, DIRECTION_MAX);
    }
    else if (height == Y) {
      char bottom[] = {N, W, E, -1, NW, NE, -2, -2};
      memcpy(buf0, bottom, DIRECTION_MAX);
    }
    else {
      char center[] = {N, W, E, S, NW, NE, SW, SE};
      memcpy(buf0, center, DIRECTION_MAX);
    }
  }
}