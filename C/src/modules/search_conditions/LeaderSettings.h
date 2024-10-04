#ifndef _LEADER_SETTINGS_H_
#define _LEADER_SETTINGS_H_

#include <stdbool.h>
#include "../../constants.h"
#include "../io/Parser.h"

// リーダーに関する設定クラス
typedef struct {
  char leader1;             // 自分のリーダー
  char leader2;             // フレンドのリーダー
  char maxCombo;            // 現在の盤面で可能な最大コンボ数
  double maxMagnification;  // 現在の盤面で可能な最大倍率
  bool isComboLeader;       // コンボ系のリーダーかどうか
  bool isMagniLeader;       // 高倍率系のリーダーかどうか
  bool isCrossLeader;       // 十字消し系のリーダーかどうか
} LeaderSettings;

// 初期化関数
extern void LeaderSettings_init(LeaderSettings* this, char request[]);

// 自分のリーダーを取得する関数
extern char LeaderSettings_getLeader1(LeaderSettings* this);

// フレンドのリーダーを取得する関数
extern char LeaderSettings_getLeader2(LeaderSettings* this);

// リーダーの設定が、コンボ系のリーダーかどうかを返す関数
extern bool LeaderSettings_isComboLeader(LeaderSettings* this);

// リーダーの設定が、高倍率系のリーダーかどうかを返す関数
extern bool LeaderSettings_isMagniLeader(LeaderSettings* this);

// リーダーの設定が、十字消し系のリーダーかどうかを返す関数
extern bool LeaderSettings_isCrossLeader(LeaderSettings* this);

// 引数のコンボ数が、最大コンボ数かどうかを判定する関数
extern bool LeaderSettings_isMaxCombo(LeaderSettings* this, const char combo);

// 引数の倍率が、最大倍率かどうかを判定する関数
extern bool LeaderSettings_isMaxMagnification(LeaderSettings* this, const double magnification);

#endif  // _LEADER_SETTINGS_H_