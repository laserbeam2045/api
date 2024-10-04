#ifndef _WINSOCK_H_
#define _WINSOCK_H_

#include <stdio.h>
#include <winsock2.h>

// WinSockを扱うクラス
typedef struct {
  SOCKET sock0;
  SOCKET sock;
} WinSock;

// 初期化関数
extern void WinSock_init(WinSock *this);

// 後始末関数
extern void WinSock_finish(WinSock *this);

// TCPクライアントからの接続要求を受け付け、データを受信する関数
extern BOOL WinSock_accept(WinSock *this, char *buf, int buflen);

// TCPクライアントへデータを送信する関数
extern BOOL WinSock_send(WinSock *this, char *buf, int buflen);

#endif  // _WINSOCK_H_