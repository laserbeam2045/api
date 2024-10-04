#ifndef _CHECK_HOW_CLEAR_H_
#define _CHECK_HOW_CLEAR_H_

#include <stdlib.h>
#include <math.h>
#include "../ComboData.h"

// 消え方に関する判定を行う関数
extern char isHLine(char clearIndices[], char clearCount);
extern char isVLine(char clearIndices[], char clearCount);
extern char isCross(char clearIndices[], char clearCount);
extern char isL(char clearIndices[], char clearCount);
extern char is3x3(char clearIndices[], char clearCount);

// 指定された色について、コンボごとに、
// 消した数と指定された数の差の絶対値を求め、その内の最小値を返す関数
extern int getNearestClearedNum(ComboData *comboData, const char number, const char color);

#endif  // _CHECK_HOW_CLEAR_H