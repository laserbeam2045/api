#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "../SearchNode.h"

// 探索ノード構造体の中身を出力する関数（確認用）
// SearchNode *node   対象のノード
extern void printNodeData(SearchNode *node);

// ノード構造体の中身をJSON形式の文字列として書き込む関数
// char buf[]         書き込み先のbuf
// size_t bufSize     buf全体のサイズ
// SearchNode *node   データを持つ探索ノード
// double elapsedTime 経過時間
extern void writeNodeDataStr(char buf[], size_t bufSize, SearchNode *node, double elapsedTime);

#endif  // _OUTPUT_H