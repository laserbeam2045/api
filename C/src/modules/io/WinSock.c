#include "WinSock.h"


// 初期化関数
void WinSock_init(WinSock *this)
{
  WSADATA wsaData;
  struct sockaddr_in addr;
  BOOL yes = 1;

  // winsock2の初期化
  if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
    exit(1);
  }
  // TCPソケットの作成
  this->sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if (this->sock0 == INVALID_SOCKET) {
    printf("socket : %d\n", WSAGetLastError());
    exit(1);
  }
  // ソケットの設定
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.S_un.S_addr = INADDR_ANY;

  // TIME_WAIT状態でのbindを可能にするためにSO_REUSEADDRを有効にする
  setsockopt(this->sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

  // 設定をソケットにbindする
  if (bind(this->sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
    printf("bind : %d\n", WSAGetLastError());
    exit(1);
  }
  // TCPクライアントからの接続要求を待てる状態にする
  if (listen(this->sock0, 5) != 0) {
    printf("listen : %d\n", WSAGetLastError());
    exit(1);
  }
}


// 後始末関数
void WinSock_finish(WinSock *this)
{
  // winsock2の終了処理
  WSACleanup();
}


// TCPクライアントからの接続要求を受け付け、データを受信する関数
BOOL WinSock_accept(WinSock *this, char *buf, int buflen)
{
  struct sockaddr_in client;
  int addrlen = sizeof(client);

  // TCPクライアントからの接続要求を受け付ける
  this->sock = accept(this->sock0, (struct sockaddr *)&client, &addrlen);
  if (this->sock == INVALID_SOCKET) {
    printf("accept : %d\n", WSAGetLastError());
    return FALSE;
  }

  // 接続した相手のIPアドレスとポート番号を表示
  printf("accepted connection from %s, port=%d\n",
        inet_ntoa(client.sin_addr), ntohs(client.sin_port));

  // ソケット上のデータを受信し、バッファーに格納
  recv(this->sock, buf, buflen, 0);

  return TRUE;
}


// TCPクライアントへデータを送信する関数
BOOL WinSock_send(WinSock *this, char *buf, int buflen)
{
  int n;

  // 結果をクライアントへ返す
  n = send(this->sock, buf, buflen, 0);
  if (n < 1) {
    printf("send: %d\n", WSAGetLastError());
    return FALSE;
  }

  // TCPセッションの終了
  closesocket(this->sock);

  return TRUE;
}