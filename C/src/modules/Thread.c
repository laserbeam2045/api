#include "Thread.h"

// 初期化関数
void Thread_init(Thread* this, int baseIndex)
{
  this->baseIndex = baseIndex;
}

// baseIndex属性を取得する関数
int Thread_getBaseIndex(Thread* this)
{
  return this->baseIndex;
}

// nodeCount属性をセットする関数
void Thread_setNodeCount(Thread* this, int nodeCount)
{
  this->nodeCount = nodeCount;
}

// nodeCount属性をインクリメントする関数
void Thread_incrementNodeCount(Thread* this)
{
  this->nodeCount++;
}

// nodeCount属性を取得する関数
int Thread_getNodeCount(Thread* this)
{
  return this->nodeCount;
}

// 次に使用するキューのインデックスを取得する関数
int Thread_getNextIndex(Thread* this)
{
  return this->baseIndex + this->nodeCount;
}