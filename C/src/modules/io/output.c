#include <string.h>
#include "output.h"

static void writeComboDataStr(char buf[], size_t bufSize, size_t n, ComboData *data);
static void writeClearingDataStr(char buf[], size_t bufSize, size_t *n, ComboData *data, CS_TYPE type);


// ビームサーチノードの中身を出力する関数
void printNodeData(SearchNode *node)
{
  char buf[2048];

  memset(buf, 0, sizeof(buf));    
  writeNodeDataStr(buf, sizeof(buf), node, 0);
  printf(buf);
}


// ノード構造体の中身をJSON形式の文字列として書き込む関数
void writeNodeDataStr(char buf[], size_t bufSize, SearchNode *node, double elapsedTime)
{
  size_t n = strlen(buf);     // 書き込みを始める位置として使う
  int i;

  Board *board = SearchNode_getBoard(node);
  n += _snprintf(buf + n, bufSize, "{\n\"board\"\t\t\t: [");
  for (i = 0; i < Board_length; i++) {
    n += _snprintf(buf + n, bufSize, "%d", Board_getColor(board, i));
    if (i != (Board_length - 1)) {
      n += _snprintf(buf + n, bufSize, ", ");
    }
  }
  n += _snprintf(buf + n, bufSize, "],\n");

  const char *process = SearchNode_getProcess(node);
  n += _snprintf(buf + n, bufSize, "\"process\"\t\t: [");
  for (i = 0; i < PROCESS_LEN_MAX; i++) {
    n += _snprintf(buf + n, bufSize, "%d", process[i]);
    if (i != (PROCESS_LEN_MAX - 1)) {
      n += _snprintf(buf + n, bufSize, ", ");
    }
    if (i == 29) n += _snprintf(buf + n, bufSize, "\n\t\t\t   ");
  }
  n += _snprintf(buf + n, bufSize, "],\n");

  n += _snprintf(buf + n, bufSize,
    "\"movedCount\"\t\t: %d,\n"
    "\"movedCountDiagonally\"\t: %d,\n"
    "\"hashValue\"\t\t: \"%08x%08x\",\n"
    "\"elapsedTime\"\t\t: %3.2lf,\n",
    SearchNode_getMovedCount(node),
    SearchNode_getMovedCountDiagonally(node),
    (int)(*SearchNode_getHashValue(node) >> 32),
    (int)(*SearchNode_getHashValue(node)),
    elapsedTime
  );

  writeComboDataStr(buf, bufSize, n, SearchNode_getComboData(node));
  _snprintf(buf + strlen(buf), bufSize, "}\n");
}


// コンボ情報構造体の中身をJSON形式の文字列として書き込む関数
static void writeComboDataStr(char buf[], size_t bufSize, size_t n, ComboData *data)
{
  int i, j;

  n += _snprintf(buf + n, bufSize,
    "\"comboData\": {\n"
    "\t\"step\"\t\t: %d,\n",
    ComboData_getStep(data)
  );

  n += _snprintf(buf + n, bufSize, "\t\"four\"\t: [");
  writeClearingDataStr(buf, bufSize, &n, data, (CS_TYPE)FOUR);

  n += _snprintf(buf + n, bufSize, "\t\"line\"\t\t: [");
  writeClearingDataStr(buf, bufSize, &n, data, (CS_TYPE)LINE);

  n += _snprintf(buf + n, bufSize, "\t\"L\"\t\t: [");
  writeClearingDataStr(buf, bufSize, &n, data, (CS_TYPE)L);

  n += _snprintf(buf + n, bufSize, "\t\"cross\"\t\t: [");
  writeClearingDataStr(buf, bufSize, &n, data, (CS_TYPE)CROSS);

  n += _snprintf(buf + n, bufSize, "\t\"square\"\t: [");
  writeClearingDataStr(buf, bufSize, &n, data, (CS_TYPE)SQUARE);

  n += _snprintf(buf + n, bufSize, "\t\"combo\"\t: [\n");
  for (i = 0; i < DROP_TYPE_MAX + 1; i++) {
    n += _snprintf(buf + n, bufSize, "\t\t\t\t[");
    for (j = 0; j < COUNT_COMBO_MAX; j++) {
      n += _snprintf(buf + n, bufSize, "%d",
        ComboData_getComboOf(data, i, j)
      );
      if (j != (COUNT_COMBO_MAX - 1)) {
        n += _snprintf(buf + n, bufSize, ", ");
      }
    }
    if (i != DROP_TYPE_MAX)
      n += _snprintf(buf + n, bufSize, "],\n");
    else 
      n += _snprintf(buf + n, bufSize, "]\n");
  }
  n += _snprintf(buf + n, bufSize, "\t\t\t  ],\n");

  n += _snprintf(buf + n, bufSize, "\t\"leftovers\"\t: [");
  for (i = 0; i < DROP_TYPE_MAX + 1; i++) {
    n += _snprintf(buf + n, bufSize, "%d", ComboData_getLeftovers(data, i));
    if (i != DROP_TYPE_MAX) n += _snprintf(buf + n, bufSize, ", ");
  }
  n += _snprintf(buf + n, bufSize, "],\n");

  n += _snprintf(buf + n, bufSize,
    "\t\"maxConnection\"\t: %d,\n"
    "\t\"magnification\"\t: %f,\n"
    "\t\"evaluation\"\t: %f,\n"
    "\t\"fulfillConditions\"\t: %d\n}\n",
    ComboData_getMaxConnection(data),
    ComboData_getMagnification(data),
    ComboData_getEvaluation(data),
    ComboData_getFulfillConditions(data)
  );
}


// 消し方に関する情報を書き込む関数
static void writeClearingDataStr(char buf[], size_t bufSize, size_t *n, ComboData *data, CS_TYPE type)
{
  for (int i = 0; i < 8; i++) {
    *n += _snprintf(buf + *n, bufSize, "%d",
      ComboData_getClearStyle(data, type, i)
    );
    if (i != 7) *n += _snprintf(buf + *n, bufSize, ", ");
  }
  *n += _snprintf(buf + *n, bufSize, "],\n");
}