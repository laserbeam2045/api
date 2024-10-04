#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*
  HTTPリクエストをパースするモジュール
*/

// 引数の全体文字列からint型の値を取得する関数
int Parser_getInt(char *buffer, char *property);

// 引数の全体文字列からdouble型の値を取得する関数
double Parser_getDouble(char *buffer, char *property);

// 引数の全体文字列から文字列を探してarrayに格納する関数
void Parser_getString(char *buffer, char *property, char array[]);

// 引数の全体文字列から数値配列を探してarrayに格納する関数
int Parser_getIntArray(char *buffer, char *property, char array[]);

#endif  // _PARSER_H_