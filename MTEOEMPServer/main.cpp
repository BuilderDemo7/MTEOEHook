#include <iostream>
#include "ini.h"
#include <WS2tcpip.h>
#include "server.h"

#pragma comment (lib, "ws2_32.lib")

typedef struct
{
	int Multiplayer = 0; // int bool
	const char* IP = "127.0.0.0";
	short Port = 56000;
	//int HostServer = 0; // int bool
} Configuration;

static int configHandler(void* user, const char* section, const char* name,
	const char* value)
{
	Configuration* pconfig = (Configuration*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("Online", "Multiplayer")) {
		pconfig->Multiplayer = atoi(value);
	}
	else if (MATCH("Online", "IP")) {
		pconfig->IP = _strdup(value);
	}
	//else if (MATCH("Online", "HostServer")) {
	//	pconfig->HostServer = atoi(value);
	//}
	else if (MATCH("Online", "Port")) {
		pconfig->Port = (short)atoi(value);
	}
	else {
		return 0;  /* unknown section/name, error */
	}
	return 1;
}

int main()
{
	Configuration config;
	config.Multiplayer = 0;
	config.IP = "127.0.0.0";
	config.Port = 56000;
	// config.HostServer = 0;

	if (ini_parse("server.ini", configHandler, &config) < 0) {
		printf("Can't load 'server.ini' configuration file, aborting...\n");
		return 0;
	}

	printf("Creating server at address %s ...\n", config.IP);
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

	WaitForConnection();

	
	//StartUpdate();

	return 1;
}