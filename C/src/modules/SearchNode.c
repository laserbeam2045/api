#include <string.h>
#include "SearchNode.h"


// 初期化関数
void SearchNode_init(SearchNode* this, Board *board, const char position)
{
  // 各メンバ変数を初期化
  Board_init(&this->board, Board_getState(board));
  this->process[0] = position;
  this->movedCount = 0;
  this->movedCountDiagonally = 0;
  this->hashValue = ZobristHash_getHashValue(board, position);
}


// comboData属性だけを初期化する関数
void SearchNode_initComboData(SearchNode* this)
{
  ComboData_init(&this->comboData, &this->board);
}


// comboDataを除く属性を、他のオブジェクトからコピーする関数
void SearchNode_copyWithoutComboData(SearchNode* this, SearchNode *other)
{
  this->board = other->board;
  this->movedCount = other->movedCount;
  this->movedCountDiagonally = other->movedCountDiagonally;
  this->hashValue = other->hashValue;
  memcpy(this->process, other->process, this->movedCount + 1);
}


// ノードを移動させる関数
uint64_t* SearchNode_moveTo(SearchNode* this, const char nextIndex, const int direction)
{
  char currIndex = this->process[this->movedCount];
  char currColor = Board_getColor(&this->board, currIndex);
  char nextColor = Board_getColor(&this->board, nextIndex);

  // 盤面のドロップを交換する
  Board_swapColor(&this->board, currIndex, nextIndex);
  // ハッシュ値を更新する
  this->hashValue = ZobristHash_getSwappedHashValue(this->hashValue,
                            currIndex, nextIndex, currColor, nextColor);
  // 移動回数を加算する
  this->movedCount++;
  if (3 < direction) {
    this->movedCountDiagonally++;
  }
  // 手順を記録する
  this->process[this->movedCount] = nextIndex;

  // 移動後のハッシュ値を返す
  return &this->hashValue;
}


// 直前の座標を取得する関数
char SearchNode_getPreviousIndex(SearchNode* this)
{
  return this->movedCount ? this->process[this->movedCount - 1] : -1;
}


// 現在の座標を取得する関数
char SearchNode_getCurrentIndex(SearchNode* this)
{
  return this->process[this->movedCount];
}


// board属性のアドレスを取得する関数
Board* SearchNode_getBoard(SearchNode* this)
{
  return &this->board;
}


// board属性が持つ、state属性を取得する関数
const char* SearchNode_getBoardState(SearchNode* this)
{
  return Board_getState(&this->board);
}


// process属性のアドレスを取得する関数
const char* SearchNode_getProcess(SearchNode* this)
{
  return this->process;
}


// movedCount属性を取得する関数
char SearchNode_getMovedCount(SearchNode* this)
{
  return this->movedCount;
}


// movedCountDiagonally属性を取得する関数
char SearchNode_getMovedCountDiagonally(SearchNode* this)
{
  return this->movedCountDiagonally;
}


// ハッシュ値（アドレス）を取得する関数
uint64_t* SearchNode_getHashValue(SearchNode* this)
{
  return &this->hashValue;
}


// comboData属性のアドレスを取得する関数
ComboData* SearchNode_getComboData(SearchNode* this)
{
  return &this->comboData;
}