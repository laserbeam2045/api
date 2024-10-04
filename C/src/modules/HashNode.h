#ifndef _HASH_NODE_H_
#define _HASH_NODE_H_

#include <stdlib.h>
#include <stdbool.h>
#include "../../SFMT/SFMT.h"

// ハッシュ値の二分木ノードクラス
typedef struct _node{
  uint64_t value;
  struct _node *left;
  struct _node *right;
} HashNode;

// 初期化関数
// this       自身を指すアドレス
// hashValue  ノードに持たせるハッシュ値のアドレス
extern void HashNode_init(HashNode* this, const uint64_t* hashValue);

// 二分木を探索して、引数のハッシュ値が未登録なら登録し、結果を返す関数
// node       ルートノードのアドレス
// hashValue  登録するハッシュ値のアドレス
// 戻り値：true（未登録）or false（登録済み）
extern bool HashNode_makeTree(HashNode *node, const uint64_t* hashValue);

// 二分木に使用した動的メモリを開放する関数
// this       ルートノードのアドレス
extern void HashNode_finish(HashNode* this);

#endif  // _HASH_NODE_H_