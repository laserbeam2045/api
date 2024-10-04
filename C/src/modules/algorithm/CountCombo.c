#include "count_combo.h"

// クラス変数
int CountCombo_clearable;   // 消せるドロップ（ビットフラグ)
int CountCombo_droppable;   // 落ちうるドロップ（ビットフラグ）

// 初期化関数
void CountCombo_init(BoardSettings *bsp)
{
  CountCombo_clearable = BoardSettings_getClearable(bsp);
  CountCombo_droppable = BoardSettings_getDroppable(bsp);
}