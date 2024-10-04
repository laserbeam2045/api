#include "BoardSettings.h"


// 初期化関数
void BoardSettings_init(BoardSettings* this, char request[])
{
  char boardState[BOARD_LEN_MAX];
  char boardSize[6];
  char height, width;

  Parser_getIntArray(request, "board", boardState);
  Parser_getString(request, "boardSize", boardSize);
  height = boardSize[1] - '0';
  width = boardSize[3] - '0';
  
  Board_initClass(&this->board, height, width);
  Board_init(&this->board, boardState);
  this->clearable = Parser_getInt(request, "clearable");
  this->droppable = Parser_getInt(request, "fallDrops");
  this->startPosition = Parser_getInt(request, "startPosition");
  this->noEntryPositionsCount = Parser_getIntArray(request, "noEntryPositions", this->noEntryPositions);
  printf("%d", this->noEntryPositionsCount);
}

// board属性の先頭ポインタを返す関数
Board* BoardSettings_getBoard(BoardSettings* this)
{
  return &this->board;
}

// clearable属性を返す関数
int BoardSettings_getClearable(BoardSettings* this)
{
  return this->clearable;
}

// droppable属性を返す関数
int BoardSettings_getDroppable(BoardSettings* this)
{
  return this->droppable;
}

// 与えられた引数の座標が、開始位置として選択可能かどうかを判定する関数
bool BoardSettings_isUnstartable(BoardSettings* this, const char position)
{
  if (
    this->startPosition != -1 &&
    this->startPosition != position
  ) {
    return true;
  } else {
    return false;
  }
}

// 与えられた引数の座標が、操作不可地点に含まれるかどうかを判定する関数
bool BoardSettings_isNoEntryPosition(BoardSettings* this, const char position)
{
  for (char i = 0; i < this->noEntryPositionsCount; i++) {
    if (position == this->noEntryPositions[i]) {
      return true;
    }
    printf("%c", this->noEntryPositions[i]);
  }
  return false;
}