#include "ZobristHash.h"

// 局面の状態（位置×種類）に応じて排他的論理和を適用するための、乱数を保持する配列
static uint64_t ZobristHash_table[BOARD_LEN_MAX][DROP_TYPE_MAX + 1];


// 乱数テーブルを初期化する関数（最初に一度だけ呼ぶ必要がある）
void ZobristHash_init(void)
{
  // 状態を保持する構造体
  sfmt_t sfmt;

  // シードを指定して初期化
  int seed = 0;
  sfmt_init_gen_rand(&sfmt, seed);

  for (char index = 0; index < BOARD_LEN_MAX; index++)
    for (char color = 0; color < DROP_TYPE_MAX + 1; color++) {
      ZobristHash_table[index][color] = sfmt_genrand_uint64(&sfmt);
      // printf("%08x%08x\n", (int)(ZobristHash_table[index][color] >> 32),
      //                       (int)ZobristHash_table[index][color]);
    }
}

// 与えられた局面に対応するハッシュ値を取得する関数
uint64_t ZobristHash_getHashValue(Board *board, char position)
{
  uint64_t hashValue = 0;

  // 盤面を表すハッシュ値を作成する
  for (int index = 0; index < Board_length; index++) {
    char color = Board_getColor(board, index);
    hashValue ^= ZobristHash_table[index][color];
  }
  // positionのドロップを持った状態を表すハッシュ値にして返す
  return hashValue ^ ZobristHash_table[position][0];
}

// 引数のハッシュ値を、ドロップを入れ替えた状態へと更新して返す関数
uint64_t ZobristHash_getSwappedHashValue(uint64_t hashValue, char index1, char index2, char color1, char color2)
{
  hashValue ^= ZobristHash_table[index1][0];
  hashValue ^= ZobristHash_table[index2][0];
  hashValue ^= ZobristHash_table[index1][color1];
  hashValue ^= ZobristHash_table[index1][color2];
  hashValue ^= ZobristHash_table[index2][color1];
  hashValue ^= ZobristHash_table[index2][color2];

  return hashValue;
}