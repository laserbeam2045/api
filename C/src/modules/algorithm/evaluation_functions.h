#ifndef _EVALUATION_FUNCTIONS_H_
#define _EVALUATION_FUNCTIONS_H_

#include <stdlib.h>
#include "../ComboData.h"
#include "../LeaderSkill.h"
#include "../search_conditions/SearchConditions.h"
#include "check_how_clear.h"

// 共通の評価関数
// comboData *data   評価対象となるコンボ情報構造体
// searchConditions  探索条件
// moveCost          移動コスト
// 戻り値：評価値
extern double evaluate(ComboData *data, SearchConditions *searchConditions, int depth);

#endif  // _EVALUATION_FUNCTIONS_H_