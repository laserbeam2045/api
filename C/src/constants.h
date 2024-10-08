#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define SIZE_OF_CHAR 1
#define DROP_TYPE_BASIC 6     // 通常のドロップの種類数
#define DROP_TYPE_MAX 10      // 全ドロップの種類数
#define BOARD_LEN_MAX 42      // 盤面のドロップ数（6x7盤面時）
#define PROCESS_LEN_MAX 91    // 記録する手順の上限
#define COUNT_COMBO_MAX 10    // 色別に記録するコンボ数の上限
#define DIRECTION_MIN 4       // 展開する方向の数の最小値（上下左右なので４）
#define DIRECTION_MAX 8       // 展開する方向の数の最大値（上下左右＋斜めなので８）
#define STACK_NODE_MAX 100    // 保持する「優れた」ノードの数の上限
#define SIMULATION_MAX 30000  // １ノードあたりの落ちコンのシミュレーション回数

// ドロップの種類
typedef enum {
  NONE,
  FIRE,
  WATER,
  WOOD,
  LIGHT,
  DARK,
  HEART,
  BLOCK,
  POISON,
  DEADLY_POISON,
  BOMB,
} DROP_TYPE;

// リーダーの種類
typedef enum {
  ANUBIS,
  METATRON,
  KOMASAN_S,
  AMEN,
  HYLEN,
  COCO,
  VEROAH,
  YASHAMARU,
  SHIKI,
  TRAGON,
  APOLLON,
  AMAKOZUMI,
  WRATH,
  KYLO_REN,
  ALLATU,
  SUKUNA,
} LEADER;

#endif  // _CONSTANTS_H_