#pragma once
#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

// windows socket data
static WSADATA wsData;
static SOCKET serverSocket;
static WORD ver = MAKEWORD(2,2);
static sockaddr_in hint;
static sockaddr_in client;

// client info
static char host[NI_MAXHOST];    // Client's remote name
static char service[NI_MAXSERV]; // Client service (i.e. port) the client is connected on

static int playersCount = 0;
static SOCKET playersSockets[8] = {NULL};
static char* playersLastBuffer[8] = {};
static const char* playersNames[8] = {NULL};

static int lastPlayerId = -1;

//void PrepareSocket(short port, ULONG ip);
bool PrepareServer(short port, const char* ip);

bool InitializeSocket();
bool CreateSocket();
bool BindSocket(short port, ULONG ip);
bool BindSocket(short port, const char* ip);

void WaitForConnection();
void StartListeningToPlayer(int id);
void WaitForPlayerData(LPVOID sId);

void CreateNetTransmitterThreadForPlayer(int id);
void NetTransmitter(LPVOID sId);
void NetTransmit(int id);

void StartUpdate();
void UpdateServer();