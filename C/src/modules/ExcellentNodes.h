#ifndef _EXCELLENT_NODES_H_
#define _EXCELLENT_NODES_H_

#include "../constants.h"
#include "SearchNode.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
  良質なノードを保持するクラス
*/
typedef struct {
  SearchNode stack[STACK_NODE_MAX]; // ノードを保持するためのスタック
  int nextIndex;                    // 次に入れる場所(ループさせる)
  int bestIndex;                    // 最良ノードのインデックス
  int nodeCount;                    // スタックされたノードの総数
} ExcellentNodes;

/* 初期化関数 */
extern void ExcellentNodes_init(ExcellentNodes* this);

// 最良ノードをスタックに追加する関数
extern void ExcellentNodes_setNode(ExcellentNodes* this, const SearchNode* searchNode, int index);

// 最良ノードをスタックに追加する関数
// node    追加するノードのポインタアドレス
extern void ExcellentNodes_setBestNode(ExcellentNodes* this, const SearchNode *SearchNode);

// スタックされたノードの数を返す関数
extern int ExcellentNodes_getNodeCount(ExcellentNodes* this);

// 指定されたインデックスのノードのポインタアドレスを返す関数
// index     取り出す対象のインデックス
extern SearchNode* ExcellentNodes_getNodeOf(ExcellentNodes* this, const int index);

// 最良ノードの評価値を返す関数
extern double ExcellentNodes_getBestEvaluation(ExcellentNodes* this);

// 最良ノードの評価値を返す関数
extern double ExcellentNodes_getBestEvaluationProbability(ExcellentNodes* this);

// 最良ノードを返す関数
extern SearchNode ExcellentNodes_getBestNode(ExcellentNodes* this);

// スタックをソートする関数
extern void ExcellentNodes_sortNodes(ExcellentNodes* this, void* fn);

#endif  // _EXCELLENT_NODES_H_