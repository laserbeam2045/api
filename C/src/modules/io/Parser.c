#include "parser.h"

// プライベート関数
static bool Parser_getValue(char *body, const char property[], char value[]);
static int Parser_getValueIndex(char *body, const char property[]);
static int Parser_getArrayFrom(char string[], char array[]);

// プロパティをダブルクオートで囲ませるためのフォーマット
static const char QUOTES_FORMAT[] = "\"%s\"";


// 引数の全体文字列からint型の値を取得する関数
int Parser_getInt(char *buffer, char *property)
{
  char _property[32], value[256];

  sprintf(_property, QUOTES_FORMAT, property);
  Parser_getValue(buffer, _property, value);
  return atoi(value);
}

// 引数の全体文字列からdouble型の値を取得する関数
double Parser_getDouble(char *buffer, char *property)
{
  char _property[32], value[256];

  sprintf(_property, QUOTES_FORMAT, property);
  Parser_getValue(buffer, _property, value);
  return atof(value);
}

// 引数の全体文字列から文字列を探してarrayに格納する関数
void Parser_getString(char *buffer, char *property, char array[])
{
  char _property[32];

  sprintf(_property, QUOTES_FORMAT, property);
  Parser_getValue(buffer, _property, array);
}

// 引数の全体文字列から数値配列を探してarrayに格納する関数
int Parser_getIntArray(char *buffer, char *property, char array[])
{
  char _property[32], value[256];

  sprintf(_property, QUOTES_FORMAT, property);
  Parser_getValue(buffer, _property, value);
  return Parser_getArrayFrom(value, array);
}


// body文字列から、propertyを探してvalueを書き込む関数
// char body[]       全体の文字列
// char property[]   propertyが書き込まれた配列
// char value[]      valueを書き込む配列
// 戻り値：true(成功) or false(失敗)
static bool Parser_getValue(char *body, const char property[], char value[])
{
  int valueIndex = Parser_getValueIndex(body, property);

  if (valueIndex == 0) {
    return false;
  }
  int index = 0;
  char objectCount = 0;
  char arrayCount = 0;
  bool commaFlag = false;
  bool endFlag = false;

  for (int j = valueIndex, len = strlen(body); j < len; j++) {
    switch (body[j]) {
    case '{':
      objectCount++;
      break;
    case '}':
      if (objectCount) {
        objectCount--;
      } else if (0 == arrayCount) {
        endFlag = true;
      }
      break;
    case '[':
      arrayCount++;
      break;
    case ']':
      if (arrayCount) {
        arrayCount--;
      } else if (0 == objectCount) {
        endFlag = true;
      }
      break;
    case ',':
      if (0 == objectCount && 0 == arrayCount) {
        commaFlag = true;
        endFlag = true;
      }
      break;
    }
    if (!commaFlag) {
      value[index++] = body[j];
    }
    if (endFlag) break;
  }
  value[index] = '\0';

  return true;
}


// body文字列から、property文字列を探して、valueの開始位置を返す関数
// char body[]       全体の文字列
// char property[]   探すproperty
// 戻り値 int（0:失敗, 0以外:成功）
static int Parser_getValueIndex(char *body, const char property[])
{
  int propertyLen = strlen(property);
  int matchCount = 0;

  for (int i = 0, len = strlen(body); i < len; i++) {
    if (body[i] == property[matchCount]) {
      matchCount++;
      if (matchCount == propertyLen) {
        return i + 2;
      }
    } else {
      matchCount = 0;
    }
  }
  return 0;
}


// 配列形式の文字列から、数値を取り出して配列に格納する関数
// string[]   パースする文字列
// array[]    データを格納する配列
// 戻り値：配列の要素数
static int Parser_getArrayFrom(char string[], char array[])
{
  int count = 0;
  char tmpBuf[3] = {0};
  char tmpCnt = 0;

  for (int i = 0, len = strlen(string); i < len; i++) {
    if (isdigit(string[i])) {
      tmpBuf[tmpCnt] = string[i];
      tmpCnt++;
    } else if (tmpCnt) {
      array[count] = atoi(tmpBuf);
      count++;
      tmpBuf[0] = 0;
      tmpBuf[1] = 0;
      tmpCnt = 0;
    }
  }
  return count;
}