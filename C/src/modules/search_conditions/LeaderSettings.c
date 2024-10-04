#include "LeaderSettings.h"

// 初期化関数
void LeaderSettings_init(LeaderSettings* this, char request[])
{
  this->leader1 = Parser_getInt(request, "leader1");
  this->leader2 = Parser_getInt(request, "leader2");
  this->maxCombo = Parser_getInt(request, "maxCombo");
  this->maxMagnification = Parser_getDouble(request, "maxMagni");
  this->isComboLeader = false;
  this->isMagniLeader = false;
  this->isCrossLeader = false;
  
  if (
    (
      this->leader1 == (LEADER)ANUBIS &&
      this->leader2 == (LEADER)ANUBIS
    ) || (
      this->leader1 == (LEADER)METATRON &&
      this->leader2 == (LEADER)METATRON
    )
  ) {
    this->isComboLeader = true;
  }
  else if (
    (
      this->leader1 == (LEADER)AMEN &&
      this->leader2 == (LEADER)AMEN
    ) || (
      this->leader1 == (LEADER)HYLEN &&
      this->leader2 == (LEADER)HYLEN
    )
  ) {
    this->isMagniLeader = true;
  }
  else if (
    this->leader1 == (LEADER)TRAGON ||
    this->leader2 == (LEADER)TRAGON ||
    this->leader1 == (LEADER)AMAKOZUMI ||
    this->leader2 == (LEADER)AMAKOZUMI ||
    this->leader1 == (LEADER)KYLO_REN ||
    this->leader2 == (LEADER)KYLO_REN
  ) {
    this->isCrossLeader = true;
  }
}

// 自分のリーダーを取得する関数
char LeaderSettings_getLeader1(LeaderSettings* this)
{
  return this->leader1;
}

// フレンドのリーダーを取得する関数
char LeaderSettings_getLeader2(LeaderSettings* this)
{
  return this->leader2;
}

// コンボ系のリーダーかどうかを返す関数
bool LeaderSettings_isComboLeader(LeaderSettings* this)
{
  return this->isComboLeader;
}

// 高倍率系のリーダーかどうかを返す関数
bool LeaderSettings_isMagniLeader(LeaderSettings* this)
{
  return this->isMagniLeader;
}

// リーダーの設定が、十字消し系のリーダーかどうかを返す関数
bool LeaderSettings_isCrossLeader(LeaderSettings* this)
{
  return this->isCrossLeader;
}

// 引数のコンボ数が、最大コンボ数かどうかを判定する関数
bool LeaderSettings_isMaxCombo(LeaderSettings* this, const char combo)
{
  return (combo == this->maxCombo);
}

// 引数の倍率が、最大倍率かどうかを判定する関数
bool LeaderSettings_isMaxMagnification(LeaderSettings* this, const double magnification)
{
  return (magnification == this->maxMagnification);
}