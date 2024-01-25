#include <iostream>
#include "ini.h"
#include <WS2tcpip.h>
#include "server.h"

#pragma comment (lib, "ws2_32.lib")

typedef struct
{
	//int Multiplayer = 0; // int bool
	const char* IP = "127.0.0.1";
	short Port = 56000;
	bool RequestAIData = true;
	int RequestAIDataSleepTime = 50;
	//int HostServer = 0; // int bool
} Configuration;

static int configHandler(void* user, const char* section, const char* name,
	const char* value)
{
	Configuration* pconfig = (Configuration*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	//if (MATCH("Online", "Multiplayer")) {
	//	pconfig->Multiplayer = atoi(value);
	//}
	if (MATCH("Online", "IP")) {
		pconfig->IP = _strdup(value);
	}
	//else if (MATCH("Online", "HostServer")) {
	//	pconfig->HostServer = atoi(value);
	//}
	else if (MATCH("Online", "Port")) {
		pconfig->Port = (short)atoi(value);
	}
	else if (MATCH("Streaming", "RequestAIData")) {
		pconfig->RequestAIData = atoi(value) > 0 ? true : false;
	}
	else if (MATCH("Streaming", "RequestAIDataSleepTime")) {
		pconfig->RequestAIDataSleepTime = atoi(value);
	}
	else {
		return 0;  /* unknown section/name, error */
	}
	return 1;
}

int main()
{
	Configuration config;
	config.IP = "127.0.0.0";
	config.Port = 56000;
	// config.HostServer = 0;

	if (ini_parse("server.ini", configHandler, &config) < 0) {
		printf("Can't load 'server.ini' configuration file, loading default configuration ...\n");
		return 0;
	}

	printf("Creating server at %s ...\n", config.IP);
	bool serverStatus = PrepareServer(config.Port, config.IP);

	if (serverStatus)
	    printf("Success!\n");
	else
	{
		printf("Failed!\n");
		return 0;
	}

	// create a thread waiting for players data and connection
	//CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(WaitForPlayersData), nullptr, 0, nullptr);

	SetRequestAIData(config.RequestAIData);
	if (config.RequestAIData) {
		printf("WARNING: Request AI data for players is enabled, the players might have slow connections with the server.\n");
		SetRequestAIDataSleepTime(config.RequestAIDataSleepTime);
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(RequestAIVehicleDataLoop), nullptr, 0, nullptr);
	}
	WaitForConnection();

	//StartUpdate();

	return 1;
}