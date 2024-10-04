#include "ClearingSettings.h"

// 初期化関数
void ClearingSettings_init(ClearingSettings* this, char request[])
{
  this->four = Parser_getInt(request, "four");
  this->line = Parser_getInt(request, "line");
  this->L = Parser_getInt(request, "L");
  this->cross = Parser_getInt(request, "cross");
  this->square = Parser_getInt(request, "square");

  if (
    0 == this->four &&
    0 == this->line &&
    0 == this->L &&
    0 == this->cross &&
    0 == this->square
  ) {
    this->configed = false;
  } else {
    this->configed = true;
  }
}

// 1つでも設定された項目があるかどうかを返す関数
bool ClearingSettings_isConfiged(ClearingSettings* this)
{
  return this->configed;
}

// type属性のcolor番目のビットについて、フラグが立っている（設定されている）かどうかを返す関数
bool ClearingSettings_isActiveOf(ClearingSettings* this, const char type, const char color)
{
  switch (type) {
  case (CS_TYPE)FOUR     : return (1 << color) & this->four;
  case (CS_TYPE)LINE     : return (1 << color) & this->line;
  case (CS_TYPE)L        : return (1 << color) & this->L;
  case (CS_TYPE)CROSS    : return (1 << color) & this->cross;
  case (CS_TYPE)SQUARE   : return (1 << color) & this->square;
  }
}