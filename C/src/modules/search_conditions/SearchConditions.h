#ifndef _SEARCH_CONDITIONS_H_
#define _SEARCH_CONDITIONS_H_

#include "../io/Parser.h"
#include "../ComboData.h"
#include "LeaderSettings.h"
#include "BoardSettings.h"
#include "SearchSettings.h"
#include "ClearingSettings.h"
#include "SpecialSettings.h"

// 探索全般に関する設定のクラス
typedef struct {
  LeaderSettings leaderSettings;      // リーダーに関する設定
  BoardSettings boardSettings;        // 盤面に関する設定
  SearchSettings searchSettings;      // 探索に関する設定
  ClearingSettings clearingSettings;  // 消し方に関する設定
  SpecialSettings specialSettings;    // 特殊な設定
} SearchConditions;


// 初期化関数
extern void SearchConditions_init(SearchConditions* this, char request[]);

// leaderSettingsのポインタを返す関数
extern LeaderSettings* SearchConditions_getLeaderSettings(SearchConditions* this);

// boardSettingsのポインタを返す関数
extern BoardSettings* SearchConditions_getBoardSettings(SearchConditions* this);

// leaderSettingsのポインタを返す関数
extern SearchSettings* SearchConditions_getSearchSettings(SearchConditions* this);

// clearingSettingsのポインタを返す関数
extern ClearingSettings* SearchConditions_getClearingSettings(SearchConditions* this);

// specialSettingsのポインタを返す関数
extern SpecialSettings* SearchConditions_getSpecialSettings(SearchConditions* this);

// 探索を打ち切る条件を満たしているかどうかを判定する関数
// comboData    判定対象となるコンボ情報のアドレス
// 戻り値：true（条件を満たしている）or false（条件を満たしていない）
extern bool SearchConditions_isEnoughAchievement(SearchConditions* this, ComboData* comboData);

#endif  // _SEARCH_CONDITIONS_H_