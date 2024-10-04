#include "ComboData.h"


// 初期化関数
void ComboData_init(ComboData* this, Board *board)
{
  char i, j;

  for (i = 0; i < EXPAND_LENGTH; i++) {
    if (i < BASIC_LENGTH) {
      this->four[i] = 0;
      this->line[i] = 0;
      this->L[i] = 0;
      this->cross[i] = 0;
      this->square[i] = 0;
    }
    for (j = 0; j < COUNT_COMBO_MAX; j++) {
      this->combo[i][j] = 0;
    }
    this->leftovers[i] = 0;
  }
  for (i = 0; i < Board_length; i++) {
    char color = Board_getColor(board, i);
    this->leftovers[color]++;
  }
  this->leftovers[0] = Board_length;
  this->step = 0;
  this->maxConnection = 0;
  this->magnification = 1;
  this->evaluation = 0;
  this->evaluationProbability = 0;
  this->fulfillConditions = true;
  this->explosionCount = 0;
  this->additionalCombo = 0;
}

// コンボ数に指定された数をセットする関数
void ComboData_setCombo(ComboData* this, const char numberToSet)
{
  this->combo[0][0] = numberToSet;
}

// コンボ数に指定された数を加算する関数
void ComboData_addCombo(ComboData* this, const char numberToAdd)
{
  this->combo[0][0] += numberToAdd;
}

// コンボ数を加算し、関連する属性を更新する関数
void ComboData_markCombo(ComboData* this, const char dropColor, const char dropCount)
{
  // 代入する位置（加算前のコンボ数）を取得しておく
  char comboNum = this->combo[0][dropColor];

  this->combo[0][0]++;                          // コンボ数の総和を加算
  this->combo[0][dropColor]++;                  // この色についてのコンボ数を加算
  this->combo[dropColor][comboNum] = dropCount; // この色についての、このコンボで消えた数を記録
  this->leftovers[0] -= dropCount;              // 盤面に残っている、ドロップ数の総和を減算
  this->leftovers[dropColor] -= dropCount;      // 盤面に残っている、この色のドロップ数を減算

  // それまでの最大結合数を上回るなら更新する
  if (this->maxConnection < dropCount) {
    this->maxConnection = dropCount;
  }
}

// コンボ数を返す関数
double ComboData_getCombo(ComboData* this)
{
  return this->combo[0][0];
}

// step属性を加算する関数
void ComboData_incrementStep(ComboData* this)
{
  this->step++;
}

// step属性を取得する関数
char ComboData_getStep(ComboData* this)
{
  return this->step;
}

// 消し方に関する属性を加算する関数
void ComboData_incrementClearStyle(ComboData* this, const int style, const char color)
{
  switch (style) {
    case (CS_TYPE)FOUR  : this->four[color]++; break;
    case (CS_TYPE)LINE  : this->line[color]++; break;
    case (CS_TYPE)L     : this->L[color]++; break;
    case (CS_TYPE)CROSS : this->cross[color]++; break;
    case (CS_TYPE)SQUARE: this->square[color]++; break;
  }
}

// 消し方に関する属性を取得する関数
char ComboData_getClearStyle(ComboData* this, const int style, const char color)
{
  switch (style) {
    case (CS_TYPE)FOUR  : return this->four[color];
    case (CS_TYPE)LINE  : return this->line[color];
    case (CS_TYPE)L     : return this->L[color];
    case (CS_TYPE)CROSS : return this->cross[color];
    case (CS_TYPE)SQUARE: return this->square[color];
  }
}

// combo属性の指定されたインデックスの値を取得する関数
char ComboData_getComboOf(ComboData* this, const char index0, const char index1)
{
  return this->combo[index0][index1];
}


// 倍率属性に、指定された数を「セット」する関数
void ComboData_setMagnification(ComboData* this, const double value)
{
  this->magnification = value;
}

// 倍率属性に、指定された値を「かけ合わせる」関数
void ComboData_multiplyMagnification(ComboData* this, const double value)
{
  this->magnification *= value;
}

// 倍率属性を返す関数
double ComboData_getMagnification(ComboData* this)
{
  return this->magnification;
}


// 評価値属性に、指定された値を「セット」する関数
void ComboData_setEvaluation(ComboData* this, const double numberToSet)
{
  this->evaluation = numberToSet;
}

// 評価値属性に、指定された値を「加算」する関数
void ComboData_addEvaluation(ComboData* this, const double numberToAdd)
{
  this->evaluation += numberToAdd;
}

// 評価値属性を返す関数
double ComboData_getEvaluation(ComboData* this)
{
  return this->evaluation;
}


// 評価値属性に、指定された値を「セット」する関数
void ComboData_setEvaluationProbability(ComboData* this, const double numberToSet)
{
  this->evaluationProbability = numberToSet;
}

// 評価値属性に、指定された値を「加算」する関数
void ComboData_addEvaluationProbability(ComboData* this, const double numberToAdd)
{
  this->evaluationProbability += numberToAdd;
}

// 評価値属性を返す関数
double ComboData_getEvaluationProbability(ComboData* this)
{
  return this->evaluationProbability;
}


// maxConnectionを取得する関数
char ComboData_getMaxConnection(ComboData* this)
{
  return this->maxConnection;
}

// 盤面に残ったドロップ数を取得する関数
char ComboData_getLeftovers(ComboData* this, const char index)
{
  return this->leftovers[index];
}

// 盤面に残ったドロップ数を減少させる関数
void ComboData_decreaseLeftovers(ComboData* this, const char dropColor, const char dropCount)
{
  this->leftovers[0] -= dropCount;
  this->leftovers[dropColor] -= dropCount;
}

// fulfillConditions属性に引数の真偽値をセットする関数
void ComboData_setFulfillConditions(ComboData* this, bool flag)
{
  this->fulfillConditions = flag;
}

// fulfillConditions属性を取得する関数
bool ComboData_getFulfillConditions(ComboData* this)
{
  return this->fulfillConditions;
}

// explosionCount属性の数をセットする関数
void ComboData_setExplosionCount(ComboData* this, char count)
{
  this->explosionCount = count;
}

// explosionCount属性を取得する関数
char ComboData_getExplosionCount(ComboData* this)
{
  return this->explosionCount;
}

// additionalCombo属性をセットする関数
void ComboData_setAdditionalCombo(ComboData* this, char number)
{
  this->additionalCombo = number;
}

// additionalCombo属性を取得する関数
char ComboData_getAdditionalCombo(ComboData* this)
{
  return this->additionalCombo;
}