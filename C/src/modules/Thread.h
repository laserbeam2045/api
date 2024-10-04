#ifndef _THREAD_H_
#define _THREAD_H_

/*
  スレッドごとに、キューの使用するインデックスと、
  展開した探索ノードの数を管理させるためのクラス
*/
typedef struct {
  int baseIndex;  // キューの使用する領域の先頭インデックス
  int nodeCount;  // 展開した探索ノード数
} Thread;

// 初期化関数
extern void Thread_init(Thread* this, int baseIndex);

// baseIndex属性を取得する関数
extern int Thread_getBaseIndex(Thread* this);

// nodeCount属性をセットする関数
extern void Thread_setNodeCount(Thread* this, int nodeCount);

// nodeCount属性をインクリメントする関数
extern void Thread_incrementNodeCount(Thread* this);

// nodeCount属性を取得する関数
extern int Thread_getNodeCount(Thread* this);

// 次に使用するキューのインデックスを取得する関数
extern int Thread_getNextIndex(Thread* this);

#endif  // _THREAD_H_