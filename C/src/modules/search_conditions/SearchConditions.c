#include "SearchConditions.h"

// 初期化関数
void SearchConditions_init(SearchConditions* this, char request[])
{
  LeaderSettings_init(&this->leaderSettings, request);
  BoardSettings_init(&this->boardSettings, request);
  SearchSettings_init(&this->searchSettings, request);
  ClearingSettings_init(&this->clearingSettings, request);
  SpecialSettings_init(&this->specialSettings, request);
}

// leaderSettingsのポインタを返す関数
LeaderSettings* SearchConditions_getLeaderSettings(SearchConditions* this)
{
  return &this->leaderSettings;
}

// boardSettingsのポインタを返す関数
BoardSettings* SearchConditions_getBoardSettings(SearchConditions* this)
{
  return &this->boardSettings;
}

// leaderSettingsのポインタを返す関数
SearchSettings* SearchConditions_getSearchSettings(SearchConditions* this)
{
  return &this->searchSettings;
}

// clearingSettingsのポインタを返す関数
ClearingSettings* SearchConditions_getClearingSettings(SearchConditions* this)
{
  return &this->clearingSettings;
}

// specialSettingsのポインタを返す関数
SpecialSettings* SearchConditions_getSpecialSettings(SearchConditions* this)
{
  return &this->specialSettings;
}

// 探索を打ち切る条件を満たしているかどうかを判定する関数
bool SearchConditions_isEnoughAchievement(SearchConditions* this, ComboData* comboData)
{
  SpecialSettings *spsp = SearchConditions_getSpecialSettings(this);
  const bool greedy = SpecialSettings_getGreedy(spsp);
  const bool mustAllClear = SpecialSettings_getAllClear(spsp);
  const bool mustNotClear = SpecialSettings_getNotClear(spsp);

  const char combo = ComboData_getCombo(comboData);
  const char leftoversCount = ComboData_getLeftovers(comboData, (DROP_TYPE)NONE);
  const double magnification = ComboData_getMagnification(comboData);

  return (
    !greedy &&
    ComboData_getFulfillConditions(comboData) &&
    (
      (
        LeaderSettings_isComboLeader(&this->leaderSettings) &&
        LeaderSettings_isMaxCombo(&this->leaderSettings, combo)
      ) ||
      (
        LeaderSettings_isMagniLeader(&this->leaderSettings) &&
        LeaderSettings_isMaxMagnification(&this->leaderSettings, magnification)
      ) ||
      (
        mustAllClear && leftoversCount == 0
      ) ||
      (
        mustNotClear && leftoversCount == Board_length
      )
    )
  );
}