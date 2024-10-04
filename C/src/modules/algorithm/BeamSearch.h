#ifndef _BEAM_SEARCH_H_
#define _BEAM_SEARCH_H_

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES

#include <string.h>
#include <math.h>
#include <omp.h>
#include "../Adjacent.h"
#include "../HashNode.h"
#include "../Thread.h"
#include "../SearchNode.h"
#include "../ExcellentNodes.h"
#include "../search_conditions/SearchConditions.h"
#include "count_combo.h"
#include "evaluation_functions.h"

// ビームサーチクラス
typedef struct {
  bool endFlag;                   // 探索を途中で終了するフラグ
  int droppable;                  // 落ちうるドロップ（=落ちコンをシミュレートするかどうか）
  int beamWidth;                  // 探索のビーム幅
  int beamDepth;                  // 探索の深さ
  int maxThreads;                 // 使用可能なスレッド数
  int parentsCount;               // 親となるノードの数
  int childrenCount;              // 展開したノード数の総和
  HashNode *rootHashNode;         // ２分探索木のルートノード
  Thread *threads;                // スレッドオブジェクト
  SearchNode *parents;            // 親ノードの実データを入れるキュー
  SearchNode *children;           // 子ノードの実データを入れるキュー
  SearchNode **parentsP;          // 親ノードのポインタを保持するポインタ
  SearchNode **childrenP;         // 子ノードのポインタを保持するポインタ
  ExcellentNodes excellentNodes;  // 良質ノード集積オブジェクト
} BeamSearch;

// 初期化関数
extern void BeamSearch_init(BeamSearch* this, SearchConditions *searchConditions);

// 後始末関数
extern void BeamSearch_finish(BeamSearch* this);

// ビームサーチを実行する関数
// *searchConditions  探索条件オブジェクトのアドレス
// 戻り値：最良ノード
extern SearchNode BeamSearch_run(BeamSearch* this, SearchConditions *searchConditions);

#endif  // _USE_MATH_DEFINES
#endif  // _BEAM_SEARCH_H_