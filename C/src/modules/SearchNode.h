#ifndef _SEARCH_NODE_H_
#define _SEARCH_NODE_H_

#include "../../SFMT/SFMT.h"
#include "../constants.h"
#include "Board.h"
#include "ZobristHash.h"
#include "ComboData.h"
#include "search_conditions/BoardSettings.h"

/*
  ビーム探索のノードクラス
*/
typedef struct {
  Board board;                    // 盤面オブジェクト
  char process[PROCESS_LEN_MAX];  // 移動した経路
  char movedCount;                // 移動した回数    
  char movedCountDiagonally;      // 斜め移動した回数
  uint64_t hashValue;             // 局面を表すハッシュ値
  ComboData comboData;            // コンボ情報オブジェクト
} SearchNode;

// 初期化関数
// board          初期盤面を持つBoardインスタンス
// position       操作を開始する位置の座標
extern void SearchNode_init(SearchNode* this, Board *board, const char position);

// comboData属性だけを初期化する関数
extern void SearchNode_initComboData(SearchNode* this);

// comboDataを除く属性を、他のオブジェクトからコピーする関数
extern void SearchNode_copyWithoutComboData(SearchNode* this, SearchNode *other);

// ノードを移動させる関数
// nextIndex   移動先の座標
// direction   斜め移動かどうかの判定用（4以上なら斜め移動）
// 戻り値：移動後のハッシュ値のアドレス
extern uint64_t* SearchNode_moveTo(SearchNode* this, const char nextIndex, const int direction);

// 直前の座標を取得する関数
// 戻り値：直前の座標（なければ-1）
extern char SearchNode_getPreviousIndex(SearchNode* this);

// 現在の座標を取得する関数
// 戻り値：現在の座標
extern char SearchNode_getCurrentIndex(SearchNode* this);

// board属性のアドレスを取得する関数
extern Board* SearchNode_getBoard(SearchNode* this);

// board属性が持つ、state属性を取得する関数
extern const char* SearchNode_getBoardState(SearchNode* this);

// process属性のアドレスを取得する関数
extern const char* SearchNode_getProcess(SearchNode* this);

// movedCount属性を取得する関数
extern char SearchNode_getMovedCount(SearchNode* this);

// movedCountDiagonally属性を取得する関数
extern char SearchNode_getMovedCountDiagonally(SearchNode* this);

// ハッシュ値のアドレスを取得する関数
extern uint64_t* SearchNode_getHashValue(SearchNode* this);

// comboData属性のアドレスを取得する関数
extern ComboData* SearchNode_getComboData(SearchNode* this);

#endif  // _SEARCH_NODE_H_