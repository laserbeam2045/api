#include "SearchSettings.h"

// 初期化関数
void SearchSettings_init(SearchSettings* this, char request[])
{
  this->beamWidth = Parser_getInt(request, "beamWidth");
  this->beamDepth = Parser_getInt(request, "beamDepth");
  this->diagonalLimit = Parser_getInt(request, "diagonalLimit");
  this->additionalCombo = Parser_getInt(request, "additionalCombo");
}

// beamWidth属性を取得する関数
int SearchSettings_getBeamWidth(SearchSettings* this)
{
  return this->beamWidth;
}

// beamDepth属性を取得する関数
int SearchSettings_getBeamDepth(SearchSettings* this)
{
  return this->beamDepth;
}

// diagonalLimit属性を取得する関数
char SearchSettings_getDiagonalLimit(SearchSettings* this)
{
  return this->diagonalLimit;
}

// additionalCombo属性を取得する関数
char SearchSettings_getAdditionalCombo(SearchSettings* this)
{
  return this->additionalCombo;
}