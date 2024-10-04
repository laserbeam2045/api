#include "SpecialSettings.h"

// 初期化関数
void SpecialSettings_init(SpecialSettings* this, char request[])
{
  this->greedy = Parser_getInt(request, "greedy");
  this->allClear = Parser_getInt(request, "allClear");
  this->notClear = Parser_getInt(request, "notClear");
}

// greedy属性を返す関数
bool SpecialSettings_getGreedy(SpecialSettings* this)
{
  return this->greedy;
}

// allClear属性を返す関数
bool SpecialSettings_getAllClear(SpecialSettings* this)
{
  return this->allClear;
}

// notClear属性を返す関数
bool SpecialSettings_getNotClear(SpecialSettings* this)
{
  return this->notClear;
}
