#ifndef _SPECIAL_SETTINGS_H_
#define _SPECIAL_SETTINGS_H_

#include <stdbool.h>
#include "../../constants.h"
#include "../io/Parser.h"

// 盤面に関する設定
typedef struct {
  bool greedy;                          // 最後まで探索するかどうか
  bool allClear;                        // 全てのドロップを消すかどうか
  bool notClear;                        // 一つもドロップを消さないかどうか
} SpecialSettings;

// 初期化関数
extern void SpecialSettings_init(SpecialSettings* this, char request[]);

// greedy属性を返す関数
extern bool SpecialSettings_getGreedy(SpecialSettings* this);

// allClear属性を返す関数
extern bool SpecialSettings_getAllClear(SpecialSettings* this);

// notClear属性を返す関数
extern bool SpecialSettings_getNotClear(SpecialSettings* this);

#endif  // _SPECIAL_SETTINGS_H_