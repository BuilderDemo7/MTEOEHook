#pragma once
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

static int RecordSleepTime = 10;
static bool connected = false;

static WSADATA wsData;
static SOCKET clientSocket;
SOCKET GetClientSocket();
static WORD ver = MAKEWORD(2, 2);
static sockaddr_in hint;
static sockaddr_in client;

bool IsConnected();

bool ConnectTo(short port, const char* ip);

// client stuff
void NetRecord();
void NetSend();

void StartNetSend();
void StartNetSendLoop();
