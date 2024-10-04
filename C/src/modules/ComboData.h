#ifndef _COMBO_DATA_H_
#define _COMBO_DATA_H_

#include <stdbool.h>
#include "../constants.h"
#include "Board.h"
#include "search_conditions/ClearingSettings.h"

#define BASIC_LENGTH 8    // 基本のドロップの種類数＋２（＋２は、１Origin風に使用するためと、追い打ち用）
#define EXPAND_LENGTH 11  // 全てのドロップの種類数＋１（＋１は、１Origin風に使用するため）

// コンボ情報クラス
// MEMO: combo[0][0]はコンボ数の総和を表し、
//       combo[0][color]はその色のコンボ数を、
//       combo[color][n]はその色のn番目のコンボで消えるドロップ数を表す。
//       leftovers[color]はその色の盤面に残った数を、leftovers[0]はそれぞれの総和を表す。
typedef struct {
  char four[BASIC_LENGTH];                    // 属性ごとの二体攻撃の数
  char line[BASIC_LENGTH];                    // 属性ごとの列消しの数（縦回復を含む）
  char L[EXPAND_LENGTH];                      // 属性ごとのL字消しの数
  char cross[BASIC_LENGTH];                   // 属性ごとの十字消しの数
  char square[BASIC_LENGTH];                  // 属性ごとの無効貫通の数
  char leftovers[EXPAND_LENGTH];              // 属性ごとの盤面に残ったドロップ数
  char combo[EXPAND_LENGTH][COUNT_COMBO_MAX]; // 属性ごとのコンボ数と、消したドロップの数
  char maxConnection;                         // コンボした中で最も大きいドロップの繋がり
  char step;                                  // 落としの数
  double magnification;                       // 倍率
  double evaluation;                          // 評価値
  double evaluationProbability;               // 評価値（確率的）
  bool fulfillConditions;                     // 指定された条件を満たしているかどうか
  char explosionCount;                        // 爆発する爆弾の数
  char additionalCombo;                       // 加算コンボ数
} ComboData;


// 初期化関数
// board    盤面の初期配置
extern void ComboData_init(ComboData* this, Board *board);


// コンボ数に指定された数を「セット」する関数
// numberToSet  セットする数
extern void ComboData_setCombo(ComboData* this, const char numberToSet);

// コンボ数に指定された数を「加算する」関数
// numberToAdd  加算する数
extern void ComboData_addCombo(ComboData* this, const char numberToAdd);

// コンボ数を加算し、関連する属性を更新する関数
// dropColor    ドロップの色
// dropCount    消えるドロップの数
extern void ComboData_markCombo(ComboData* this, const char dropColor, const char dropCount);

// コンボ数を返す関数
extern double ComboData_getCombo(ComboData* this);


// step属性をインクリメントする関数
extern void ComboData_incrementStep(ComboData* this);

// step属性を返す関数
extern char ComboData_getStep(ComboData* this);


// 消し方に関する属性を加算する関数
// color   対象色
// style   消し方を表すフラグ
extern void ComboData_incrementClearStyle(ComboData* this, const int style, const char color);

// 消し方に関する属性を取得する関数
// color   対象色
// style   消し方を表すフラグ
extern char ComboData_getClearStyle(ComboData* this, const int style, const char color);


// combo属性の指定されたインデックスの値を取得する関数
// index0    1つ目の添え字に指定する値
// index1    2つ目の添え字に指定する値
extern char ComboData_getComboOf(ComboData* this, const char index0, const char index1);


// 倍率属性に、指定された数を「セット」する関数
// value  セットする値
extern void ComboData_setMagnification(ComboData* this, const double value);

// 倍率属性に、指定された値を「掛け合わせる」関数
// value  掛け合わせる値
extern void ComboData_multiplyMagnification(ComboData* this, const double value);

// 倍率属性を返す関数
extern double ComboData_getMagnification(ComboData* this);


// 評価値属性に、指定された値を「セット」する関数
// numberToSet  セットする値
extern void ComboData_setEvaluation(ComboData* this, const double numberToSet);

// 評価値属性に、指定された値を「加算」する関数
// numberToAdd  加算する値
extern void ComboData_addEvaluation(ComboData* this, const double numberToAdd);

// 評価値属性を返す関数
extern double ComboData_getEvaluation(ComboData* this);


// 評価値属性に、指定された値を「セット」する関数
// numberToSet  セットする値
extern void ComboData_setEvaluationProbability(ComboData* this, const double numberToSet);

// 評価値属性に、指定された値を「加算」する関数
// numberToAdd  加算する値
extern void ComboData_addEvaluationProbability(ComboData* this, const double numberToAdd);

// 評価値属性を返す関数
extern double ComboData_getEvaluationProbability(ComboData* this);


// maxConnectionを取得する関数
extern char ComboData_getMaxConnection(ComboData* this);

// 盤面に残ったドロップ数を取得する関数
// index  添え字に指定する値
extern char ComboData_getLeftovers(ComboData* this, const char index);

// 盤面に残ったドロップ数を減少させる関数
extern void ComboData_decreaseLeftovers(ComboData* this, const char dropColor, const char dropCount);

// fulfillConditions属性に引数の真偽値をセットする関数
extern void ComboData_setFulfillConditions(ComboData* this, bool flag);

// fulfillConditions属性を取得する関数
extern bool ComboData_getFulfillConditions(ComboData* this);

// explosionCount属性の数をセットする関数
extern void ComboData_setExplosionCount(ComboData* this, char count);

// explosionCount属性を取得する関数
extern char ComboData_getExplosionCount(ComboData* this);

// additionalCombo属性をセットする関数
extern void ComboData_setAdditionalCombo(ComboData* this, char number);

// additionalCombo属性を取得する関数
extern char ComboData_getAdditionalCombo(ComboData* this);

#endif //_COMBO_DATA_H_