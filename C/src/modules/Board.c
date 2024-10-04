#include <string.h>
#include "Board.h"

// 盤面のサイズに関するクラス属性を初期化する関数（最初に一度だけ呼ぶ必要がある）
void Board_initClass(Board* this, const char height, const char width)
{
  Board_height = height;
  Board_width = width;
  Board_length = height * width;
}

// 盤面の状態に関する初期化関数（インスタンスごとに呼ぶ）
void Board_init(Board* this, const char state[])
{
  memcpy(&this->state, state, Board_length); 
}

// state属性の指定されたインデックスに、指定された値をセットする関数
void Board_setColor(Board* this, const char index, const char color)
{
  this->state[index] = color;
}

// state属性の指定されたインデックスの値を返す関数
char Board_getColor(Board* this, const char index)
{
  return this->state[index];
}

// 指定されたインデックスの要素同士を入れ替える関数
void Board_swapColor(Board* this, const char index1, const char index2)
{
  const char temp = this->state[index1];

  this->state[index1] = this->state[index2];
  this->state[index2] = temp;
}

// state属性の先頭ポインタを返す関数
const char* Board_getState(Board* this)
{
  return this->state;
}

// クラス属性
char Board_height;
char Board_width;
char Board_length;