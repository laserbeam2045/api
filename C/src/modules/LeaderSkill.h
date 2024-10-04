#ifndef _LEADER_SKILL_H_
#define _LEADER_SKILL_H_

#include <math.h>
#include "ComboData.h"

/*
  リーダースキルクラス
*/

// リーダースキルで加算されるコンボ数を取得する関数
// leader       リーダーの識別番号
// *comboData   コンボ情報オブジェクトのアドレス
extern char LeaderSkill_getAdditionalCombo(char leader, ComboData *comboData);

// リーダースキルで適用される倍率を取得する関数
// leader       リーダーの識別番号
// *comboData   コンボ情報オブジェクトのアドレス
extern double LeaderSkill_getMagnification(char leader, ComboData *comboData);

#endif  // _LEADER_SKILL_H_