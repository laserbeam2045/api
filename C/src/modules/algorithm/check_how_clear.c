#include "check_how_clear.h"

static int asc(const void *a, const void *b);


// 横一列消しかどうかの判定
char isHLine(char clearIndices[], char clearCount)
{
  int i, number[6] = {};

  for (i = clearCount; i--;) {
    char Y = floor(clearIndices[i] / Board_width);
    number[Y]++;
  }
  for (i = 0; i < 6; i++)
    if (number[i] == Board_width) return 1;

  return 0;
}


// 縦一列消しかどうかの判定
char isVLine(char clearIndices[], char clearCount)
{
  for (char i = clearCount - 1; i--;) {
    char X1 = clearIndices[i] % Board_width,
         X2 = clearIndices[i + 1] % Board_width;
    if (X1 != X2) return 0;
  }
  return 1;
}


// 十字消しかどうかの判定
char isCross(char clearIndices[], char clearCount)
{
  qsort(clearIndices, clearCount, SIZE_OF_CHAR, asc);

  char center = clearIndices[2];

  if (clearIndices[0] == center - Board_width)
    return 1;
  else
    return 0;
}


// L字消しかどうかの判定
char isL(char clearIndices[], char clearCount)
{
  qsort((void*)clearIndices, clearCount, SIZE_OF_CHAR, asc);

  if (clearIndices[0] + 1 == clearIndices[1] &&
      clearIndices[1] + 1 == clearIndices[2])
  {
    if (clearIndices[2] + Board_width == clearIndices[3] &&
        clearIndices[3] + Board_width == clearIndices[4])   // ┐
        return 1;
    if (clearIndices[0] + Board_width == clearIndices[3] &&
        clearIndices[3] + Board_width == clearIndices[4])   // ┌
        return 1;
  } else if (clearIndices[0] + Board_width == clearIndices[1]) {
    if (clearIndices[1] + Board_width == clearIndices[2] &&
        clearIndices[2] + 1 == clearIndices[3] &&
        clearIndices[3] + 1 == clearIndices[4]) // └
        return 1;
    if (clearIndices[2] + 1 == clearIndices[3] &&
        clearIndices[3] + 1 == clearIndices[4] &&
        clearIndices[1] + Board_width == clearIndices[4]) // ┘
        return 1;
  }
  return 0;
}


// 3x3消しかどうかの判定
char is3x3(char clearIndices[], char clearCount)
{
  if (clearCount != 9) return 0;
  qsort(clearIndices, clearCount, SIZE_OF_CHAR, asc);

  if (clearIndices[0] + 1 == clearIndices[1] &&
      clearIndices[1] + 1 == clearIndices[2] &&
      clearIndices[0] + Board_width == clearIndices[3] &&
      clearIndices[3] + 1 == clearIndices[4] &&
      clearIndices[4] + 1 == clearIndices[5] &&
      clearIndices[3] + Board_width == clearIndices[6] &&
      clearIndices[6] + 1 == clearIndices[7] &&
      clearIndices[7] + 1 == clearIndices[8])
  {
    return 1;
  } else {
    return 0;
  }
}


// 指定された色について、コンボごとに、
// 消した数と指定された数の差の絶対値を求め、最小値を返す関数
int getNearestClearedNum(ComboData *comboData, const char number, const char color)
{
  int clearedNum, diff, min = 999;

  for (char i = 0; i < COUNT_COMBO_MAX; i++) {
    clearedNum = ComboData_getComboOf(comboData, color, i);
    if (0 == clearedNum) {
      break;
    }
    diff = abs(clearedNum - number);
    if (diff < min) {
      min = diff;
    }
  }
  return min;
}


// qsortの条件（昇順）
static int asc(const void *a, const void *b)
{
  return *(char*)a - *(char*)b;
}