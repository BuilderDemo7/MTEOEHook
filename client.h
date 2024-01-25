#pragma once
#include <WS2tcpip.h>
#include "CVehicle.h"

#pragma comment (lib, "ws2_32.lib")

static int RecordSleepTime = 1;
static float LastReceiveTime;
static bool connected = false;

static bool serverAskedMEForAIVehicleData = false;

void SendAIVehicleDatas();
void SendVehicleData(CVehicle* playerVeh, bool AI);
void RecordVehicleData(char buf[256], bool disableAI);

static WSADATA wsData;
static SOCKET clientSocket;
SOCKET GetClientSocket();
static WORD ver = MAKEWORD(2, 2);
static sockaddr_in hint;
static sockaddr_in client;

static int PlayersCount = 1; // 1 is for you

bool IsConnected();

bool ConnectTo(short port, const char* ip);

// client stuff
void SubNetRecord();
void NetRecord();
void NetRecord_TimerStart();

void NetSend();

void StartNetSend();
void StartNetSendLoop();