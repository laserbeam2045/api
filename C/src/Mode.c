#include "Mode.h"

static void Mode_onHttpRequest(Mode*, char[], char[], size_t);


// 初期化関数
void Mode_init(Mode* c)
{
  // WinSockオブジェクトの初期化
  WinSock_init(&c->winSock);

  // 隣接点テーブルの初期化
  Adjacent_init();

  // 乱数テーブルの初期化
  ZobristHash_init();
}


// 後始末関数
void Mode_finish(Mode* c)
{
  WinSock_finish(&c->winSock);
}


// モード関数
void Mode_run(Mode* c)
{
  char requestBuffer[2048];
  char responseBuffer[2048];

  printf("C server listening...\n\n");

  for (;;) {
    memset(requestBuffer, 0, sizeof(requestBuffer));
    memset(responseBuffer, 0, sizeof(responseBuffer));
    _snprintf(responseBuffer, sizeof(responseBuffer),
      "HTTP/1.1 200 OK\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "Content-Type: application/json\r\n\r\n");

    // TCPクライアントからの接続要求を受け付ける
    if (!WinSock_accept(&c->winSock, requestBuffer, (int)sizeof(requestBuffer))) {
      break;
    }
    // リクエストが来たら探索処理を行い、結果をレスポンスBODYとしてbufに書き込む
    Mode_onHttpRequest(c, requestBuffer, responseBuffer, sizeof(responseBuffer));

    // 結果をクライアントへ返す
    if (!WinSock_send(&c->winSock, responseBuffer, (int)strlen(responseBuffer))) {
      break;
    }
  }
}


// イベントハンドラ
static void Mode_onHttpRequest(Mode* c, char requestBuffer[],
                          char responseBuffer[], size_t responseBufferSize)
{
  // HTTPリクエストのBODYをパースし、探索条件を初期化する
  SearchConditions_init(&c->searchConditions, requestBuffer);
  // 使用する隣接点テーブルを設定する
  Adjacent_initTablePointer(Board_length);
  // 探索の前処理を行う
  BeamSearch_init(&c->beamSearch, &c->searchConditions);

  // ビームサーチを実行し、最良ノードを得る
  double startTime = omp_get_wtime();
  SearchNode bestNode = BeamSearch_run(&c->beamSearch, &c->searchConditions);
  double elapsedTime = omp_get_wtime() - startTime;

  // 探索で使用したメモリを開放させる
  BeamSearch_finish(&c->beamSearch);
  // レスポンス用のバッファーに、JSON形式で、探索結果のデータを書き込む
  writeNodeDataStr(responseBuffer, responseBufferSize, &bestNode, elapsedTime);
}