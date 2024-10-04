#include "HashNode.h"

// プライベート定数
static const int SIZE_OF_HASH_NODE = sizeof(HashNode);
// プライベート関数
static HashNode* HashNode_newNode(const uint64_t *hashValue);


// 初期化関数
void HashNode_init(HashNode* this, const uint64_t *hashValue)
{
  this->value = *hashValue;
  this->left = NULL;
  this->right = NULL;
}

// 二分木を探索して、引数のハッシュ値が未登録なら登録し、結果を返す関数
bool HashNode_makeTree(HashNode* node, const uint64_t *hashValue)
{
  for (;;) {
    if (node->value == *hashValue) {
      return false;
    } else if (node->value > *hashValue) {
      if (node->left == NULL) {
        node->left = HashNode_newNode(hashValue);
        return true;
      } else {
        node = node->left;
      }
    } else if (node->value < *hashValue) {
      if (node->right == NULL) {
        node->right = HashNode_newNode(hashValue);
        return true;
      } else {
        node = node->right;
      }
    }
  }
}

// 二分木に使用した動的メモリを開放する関数
void HashNode_finish(HashNode* this)
{
  if (this->left != NULL)
    HashNode_finish(this->left);

  if (this->right != NULL)
    HashNode_finish(this->right);

  free(this);
}

// 新しいインスタンスを作成し、ポインタアドレスを返す関数
static HashNode* HashNode_newNode(const uint64_t *hashValue)
{
  HashNode *node = malloc(SIZE_OF_HASH_NODE);
  HashNode_init(node, hashValue);

  return node;
}