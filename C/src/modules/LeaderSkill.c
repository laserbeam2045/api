#include "LeaderSkill.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

// プライベート関数
static double LeaderSkill_getMagnificationAnubis(ComboData *comboData);
static double LeaderSkill_getMagnificationMetatron(ComboData *comboData);
static double LeaderSkill_getMagnificationKomasan(ComboData *comboData);
static double LeaderSkill_getMagnificationAmen(ComboData *comboData);
static double LeaderSkill_getMagnificationHylen(ComboData *comboData);
static double LeaderSkill_getMagnificationCoco(ComboData *comboData);
static double LeaderSkill_getMagnificationVeroah(ComboData *comboData);
static double LeaderSkill_getMagnificationYashamaru(ComboData *comboData);
static double LeaderSkill_getMagnificationShiki(ComboData *comboData);
static double LeaderSkill_getMagnificationTragon(ComboData *comboData);
static double LeaderSkill_getMagnificationApollon(ComboData *comboData);
static double LeaderSkill_getMagnificationAmakozumi(ComboData *comboData);
static double LeaderSkill_getMagnificationWrath(ComboData *comboData);
static double LeaderSkill_getMagnificationKyloRen(ComboData *comboData);
static double LeaderSkill_getMagnificationAllatu(ComboData *comboData);


// リーダースキルで加算されるコンボ数を取得する関数
char LeaderSkill_getAdditionalCombo(char leader, ComboData *comboData)
{
  char flag1 = 0, flag2 = 0;
  char colorCount = 0;

  switch (leader) {
  case (LEADER)HYLEN:
    for (int i = 0; i < COUNT_COMBO_MAX; i++) {
      if (7 <= ComboData_getComboOf(comboData, (DROP_TYPE)FIRE, i))
        flag1 = 1;
      if (7 <= ComboData_getComboOf(comboData, (DROP_TYPE)WATER, i))
        flag2 = 1;
    }
    if (flag1 && flag2) {
      return 3;
    }
    break;
  case (LEADER)VEROAH:
    for (int i = 0; i < COUNT_COMBO_MAX; i++) {
      if (9 <= ComboData_getComboOf(comboData, (DROP_TYPE)WATER, i)) {
        return 3;
      }
    }
    break;
  case (LEADER)TRAGON:
    for (int i = (DROP_TYPE)FIRE; i <= (DROP_TYPE)DARK; i++) {
      if (ComboData_getComboOf(comboData, (DROP_TYPE)NONE, i)) {
        colorCount++;
      }
    }
    if (4 <= colorCount) {
      return 2;
    }
    break;
  case (LEADER)AMAKOZUMI:
    if (
      ComboData_getComboOf(comboData, (DROP_TYPE)NONE, (DROP_TYPE)LIGHT) &&
      ComboData_getComboOf(comboData, (DROP_TYPE)NONE, (DROP_TYPE)DARK)
    ) {
      return 3;
    }
    break;
  }
  return 0;
}

// リーダースキルで適用される倍率を取得する関数
double LeaderSkill_getMagnification(char leader, ComboData *comboData)
{
  double magnification;

  switch (leader) {
    case (LEADER)ANUBIS   : magnification = LeaderSkill_getMagnificationAnubis(comboData); break;
    case (LEADER)METATRON : magnification = LeaderSkill_getMagnificationMetatron(comboData); break;
    case (LEADER)KOMASAN_S: magnification = LeaderSkill_getMagnificationKomasan(comboData); break;
    case (LEADER)AMEN     : magnification = LeaderSkill_getMagnificationAmen(comboData); break;
    case (LEADER)HYLEN    : magnification = LeaderSkill_getMagnificationHylen(comboData); break;
    case (LEADER)COCO     : magnification = LeaderSkill_getMagnificationCoco(comboData); break;
    case (LEADER)VEROAH   : magnification = LeaderSkill_getMagnificationVeroah(comboData); break;
    case (LEADER)YASHAMARU: magnification = LeaderSkill_getMagnificationYashamaru(comboData); break;
    case (LEADER)SHIKI    : magnification = LeaderSkill_getMagnificationShiki(comboData); break;
    case (LEADER)TRAGON   : magnification = LeaderSkill_getMagnificationTragon(comboData); break;
    case (LEADER)APOLLON  : magnification = LeaderSkill_getMagnificationApollon(comboData); break;
    case (LEADER)AMAKOZUMI: magnification = LeaderSkill_getMagnificationAmakozumi(comboData); break;
    case (LEADER)WRATH    : magnification = LeaderSkill_getMagnificationWrath(comboData); break;
    case (LEADER)KYLO_REN : magnification = LeaderSkill_getMagnificationKyloRen(comboData); break;
    case (LEADER)ALLATU   : magnification = LeaderSkill_getMagnificationAllatu(comboData); break;
  }
  return magnification;
}

// リーダー「転生アヌビス」の倍率を取得する関数
// 8コンボ以上で攻撃力が上昇、最大10倍。
// スキル使用時、攻撃力が3倍、回復力は1.5倍。
static double LeaderSkill_getMagnificationAnubis(ComboData *comboData)
{
  char comboNum = ComboData_getCombo(comboData);

  if (7 >= comboNum)      return 3;     // 3 * 1
  else if (8 == comboNum) return 15;    // 3 * 5
  else if (9 == comboNum) return 22.5;  // 3 * 7.5
  else                    return 30;    // 3 * 10
}

// リーダー「極醒メタトロン」の倍率を取得する関数
// HP80%以下で攻撃力が3倍。神タイプのHPと攻撃力が2倍。
// 7コンボ以上でダメージを軽減、攻撃力が2.5倍。
static double LeaderSkill_getMagnificationMetatron(ComboData *comboData)
{
  char comboNum = ComboData_getCombo(comboData);
  double magnification = 2;

  if (7 <= comboNum) {
    magnification *= 2.5;
  }
  return magnification;
}

// リーダー「コマさんS」の倍率を取得する関数
// 火と木属性のHPが1.5倍。6コンボ以上で攻撃力が5倍。
// ドロップを5個以上つなげて消すとダメージを軽減、攻撃力が3倍。
static double LeaderSkill_getMagnificationKomasan(ComboData *comboData)
{
  char comboNum = ComboData_getCombo(comboData);
  char maxConnection = ComboData_getMaxConnection(comboData);
  double magnification = 1;

  if (6 <= comboNum)
    magnification *= 5;
  if (5 <= maxConnection)
    magnification *= 3;

  return magnification;
}

// リーダー「アメン」の倍率を取得する関数
//【落ちコンなし】 7コンボちょうどで攻撃力が10倍。
// パズル後の残りドロップ数が3個以下で攻撃力が10倍。
static double LeaderSkill_getMagnificationAmen(ComboData *comboData)
{
  char comboNum = ComboData_getCombo(comboData);
  char leftovers = ComboData_getLeftovers(comboData, 0);
  double magnification = 1;

  if (7 == comboNum)
    magnification *= 10;
  if (3 >= leftovers)
    magnification *= 10;

  return magnification;
}

// リーダー「ハイレン」の倍率を取得する関数
// 水属性のHPが2倍。10コンボ以上で攻撃力が30倍。
// 火と水を同時に7個以上つなげて消すと、3コンボ加算。
static double LeaderSkill_getMagnificationHylen(ComboData *comboData)
{
  char comboNum = ComboData_getCombo(comboData);

  if (10 <= comboNum) {
    return 30;
  } else {
    return 1;
  }
}

// リーダー「ココ・フェルケナ」の倍率を取得する関数
// 水属性の攻撃力が4倍。6コンボ以上でダメージを半減。
// 水を6個以上つなげて消すと攻撃力が4倍、固定5万ダメージ。
static double LeaderSkill_getMagnificationCoco(ComboData *comboData)
{
  double magnification = 4;

  for (char i = 0; i <= COUNT_COMBO_MAX; i++) {
    char clearedNum = ComboData_getComboOf(comboData, (DROP_TYPE)WATER, i);
    if (6 <= clearedNum) {
      magnification *= 4;
      break;
    }
  }
  return magnification;
}

// リーダー「水着ヴェロア」の倍率を取得する関数
// 水属性のHPが2倍、攻撃力は6倍。ドロップ操作を2秒延長。
// 水を9個以上つなげて消すと攻撃力が3倍、3コンボ加算。
static double LeaderSkill_getMagnificationVeroah(ComboData *comboData)
{
  double magnification = 6;

  for (char i = 0; i <= COUNT_COMBO_MAX; i++) {
    char clearedNum = ComboData_getComboOf(comboData, (DROP_TYPE)WATER, i);
    if (9 <= clearedNum) {
      magnification *= 3;
      break;
    }
  }
  return magnification;
}

// リーダー「鞍馬夜叉丸」の倍率を取得する関数
// 【落ちコンなし】HPと回復力が2倍。6コンボ以上で攻撃力が3倍。
// パズル後の残りドロップ数が15個以下で攻撃力が上昇、最大9倍。
static double LeaderSkill_getMagnificationYashamaru(ComboData *comboData)
{
  char comboNum = ComboData_getCombo(comboData);
  char leftovers = ComboData_getLeftovers(comboData, 0);
  double magnification = 1;

  if (6 <= comboNum)
    magnification *= 3;

  if (15 >= leftovers)
    magnification *= 9 - 0.5 * leftovers;

  return magnification;
}

// リーダー「色」の倍率を取得する関数
// 闇属性のHPが2倍。ドロップの5個L字消しで攻撃力が5倍。
// 4色以上同時攻撃で攻撃力が3倍、固定5万ダメージ。
static double LeaderSkill_getMagnificationShiki(ComboData *comboData)
{
  bool LFlag = false;
  bool cFlag = false;
  char colorCount = 0;

  for (char color = 1; color <= DROP_TYPE_MAX; color++) {
    if (LFlag == false) {
      char LCount = ComboData_getClearStyle(comboData, (CS_TYPE)L, color);
      //printf("%d\n", LCount);
      if (LCount) LFlag = true;
    }
    if (cFlag == false && color <= (DROP_TYPE)DARK) {
      if (ComboData_getComboOf(comboData, color, 0)) {
        colorCount++;
        if (4 <= colorCount) cFlag = true;
      }
    }
  }
  double magnification = 1;

  if (LFlag) magnification *= 5;
  if (cFlag) magnification *= 3;

  return magnification;
}

// リーダー「トラゴン」の倍率を取得する関数
// 4色以上同時攻撃で攻撃力が4倍、2コンボ加算。
// 5属性いずれかの5個十字消し1個につき攻撃力が3倍。
static double LeaderSkill_getMagnificationTragon(ComboData *comboData)
{
  double colorCount = 0;
  double crossCount = 0;

  for (char color = (DROP_TYPE)FIRE; color <= (DROP_TYPE)DARK; color++) {
    colorCount += ComboData_getComboOf(comboData, (DROP_TYPE)NONE, color);
    crossCount += ComboData_getClearStyle(comboData, (CS_TYPE)CROSS, color);
  }
  double magnification = 1;

  if (4 <= colorCount) magnification *= 4;
  magnification *= pow(3, crossCount);

  return magnification;
}


// リーダー「アポロン」の倍率を取得する関数
// 光属性の攻撃力が4倍。木属性のHPが2倍。
// 光木の同時攻撃で攻撃力が4倍、固定50万ダメージ。
static double LeaderSkill_getMagnificationApollon(ComboData *comboData)
{
  double magnification = 4;

  if (
    ComboData_getComboOf(comboData, (DROP_TYPE)NONE, (DROP_TYPE)WOOD) &&
    ComboData_getComboOf(comboData, (DROP_TYPE)NONE, (DROP_TYPE)LIGHT)
  ) {
    magnification *= 4;
  }

  return magnification;
}



// リーダー「アマコズミ」の倍率を取得する関数
// 【操作時間4秒】光闇の同時攻撃で攻撃力が3倍、3コンボ加算。
// 5属性いずれかの5個十字消し1個につき攻撃力が3倍。
static double LeaderSkill_getMagnificationAmakozumi(ComboData *comboData)
{
  double magnification = 1;
  double colorCount = 0;
  double crossCount = 0;

  if (
    ComboData_getComboOf(comboData, (DROP_TYPE)NONE, (DROP_TYPE)LIGHT) &&
    ComboData_getComboOf(comboData, (DROP_TYPE)NONE, (DROP_TYPE)DARK)
  ) {
    magnification *= 3;
  }
  for (char color = (DROP_TYPE)FIRE; color <= (DROP_TYPE)DARK; color++) {
    crossCount += ComboData_getClearStyle(comboData, (CS_TYPE)CROSS, color);
  }
  magnification *= pow(3, crossCount);

  return magnification;
}



// リーダー「バルディターン」の倍率を取得する関数
// 悪魔タイプのHPが2倍。HP80％以下で攻撃力が4倍。
// ドロップを5個以上つなげて消すと攻撃力が4倍、固定1ダメージ。
static double LeaderSkill_getMagnificationWrath(ComboData *comboData)
{
  if (5 <= ComboData_getMaxConnection(comboData)) {
    return 16;
  } else {
    return 4;
  }
}



// リーダー「カイロ・レン」の倍率を取得する関数
// 火闇の同時攻撃でダメージを65％軽減、攻撃力が5倍、
// 固定1ダメージ。火か闇の5個十字消し1個につき攻撃力が3倍。
static double LeaderSkill_getMagnificationKyloRen(ComboData *comboData)
{
  double magnification = 1;
  double crossCount = 0;

  if (
    ComboData_getComboOf(comboData, (DROP_TYPE)NONE, (DROP_TYPE)FIRE) &&
    ComboData_getComboOf(comboData, (DROP_TYPE)NONE, (DROP_TYPE)DARK)
  ) {
    magnification *= 5;
  }

  crossCount += ComboData_getClearStyle(comboData, (CS_TYPE)CROSS, (DROP_TYPE)FIRE);
  crossCount += ComboData_getClearStyle(comboData, (CS_TYPE)CROSS, (DROP_TYPE)DARK);

  magnification *= pow(3, crossCount);

  return magnification;
}



// リーダー「アルラトゥ」の倍率を取得する関数
// 【7x6マス】闇属性の回復力が3倍。操作時間5秒延長。
// 火か水か闇の5個十字消し1個につき攻撃力が3倍。
static double LeaderSkill_getMagnificationAllatu(ComboData *comboData)
{
  double magnification = 1;
  double crossCount = 0;

  crossCount += ComboData_getClearStyle(comboData, (CS_TYPE)CROSS, (DROP_TYPE)FIRE);
  crossCount += ComboData_getClearStyle(comboData, (CS_TYPE)CROSS, (DROP_TYPE)WATER);
  crossCount += ComboData_getClearStyle(comboData, (CS_TYPE)CROSS, (DROP_TYPE)DARK);

  magnification *= pow(3, crossCount);

  return magnification;
}
