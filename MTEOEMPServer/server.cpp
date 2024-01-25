#include <iostream>
#include <WS2tcpip.h>
#include "server.h"
#include <thread>
#include <string>

enum MultiplayerCommand : int
{
	COMMAND_PLAYERVEHICLEDATA = 0x10,

	COMMAND_PLAYERCHAT = 0x11,
	COMMAND_PLAYERDISCONNECT = 0x12,
	COMMAND_PLAYERCONNECT = 0x13,

	COMMAND_TIMEADVANCE = 0x13,

	// used for sending others AI cars data
	COMMAND_AIVEHICLEDATA = 0x14,
	COMMAND_REQUEST_AIVEHICLEDATA = 0x15,
};

#pragma comment (lib, "ws2_32.lib")

bool PrepareServer(short port, const char* ip)
{
	if (InitializeSocket()) {
		if (CreateSocket())
		{
			if (BindSocket(port, ip)) {
			     return true;
			}
		}
	}
	return false;
}

bool InitializeSocket()
{
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		printf("Something went wrong in initializing winsock!\n");
		return false;
	}
	return true;
}

bool CreateSocket()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		printf("Can't create a socket!\n");
		return false;
	}
	return true;
}

// INADDR_ANY = any 
bool BindSocket(short port, ULONG ip)
{
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = ip;

	bind(serverSocket, (sockaddr*)&hint, sizeof(hint));
	listen(serverSocket, SOMAXCONN);
	
	return true;
}

bool BindSocket(short port, const char* ip)
{
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(hint.sin_addr));

	int bindCode = bind(serverSocket, (sockaddr*)&hint, sizeof(hint));
	listen(serverSocket, SOMAXCONN);

	return bindCode == -1 ? false : true;
}

void WaitForConnection()
{
	int clientSize = sizeof(client);
	// accept connection with the client
	playersSockets[playersCount] = accept(serverSocket, (sockaddr*)&client, &clientSize );
	if (playersSockets[playersCount] == INVALID_SOCKET)
	{
		printf("Wait For Connection - Client socket is invalid\n");
		return;
	}

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		char name[255];
		ZeroMemory(name, 255);
		int bytesrecevied = recv(playersSockets[playersCount], name, 255, 0);
		// store the name
		playersNames[playersCount] = name;
		MessageConnectForPlayers(name, playersCount);

		printf("Player %s (%d) connected from port %s (%p)\n", /*host*/name, playersCount, service, playersSockets[playersCount]);
		
		//CreateNetTransmitterThreadForPlayer(playersCount);
		StartListeningToPlayer(playersCount);

		playersCount++;
	}
	else
	{
		char name[255];
		ZeroMemory(name, 255);
		int bytesrecevied = recv(playersSockets[playersCount], name, 255, 0 );
		// store the name
		playersNames[playersCount] = name;
		MessageConnectForPlayers(name, playersCount);

		printf("Player %s (%d) connected from port %d (%p)\n", /*host*/name, playersCount, ntohs(client.sin_port), playersSockets[playersCount]);

		//CreateNetTransmitterThreadForPlayer(playersCount);
		StartListeningToPlayer(playersCount);

		playersCount++;
	}
	// wait for another player
	WaitForConnection();
}

void AdvanceTimerForPlayer(int id)
{
	char buf[]
	{
		COMMAND_TIMEADVANCE
	};
	send(playersSockets[id], /*playersLastBuffer[playerid]*/buf, 4, 0);
}

void DisposePlayer(int id)
{
	closesocket(playersSockets[id]);

	playersSockets[id] = NULL;
	playersLastBuffer[id] = NULL;
	playersNames[id] = NULL;
}

// OLD
void CreateNetTransmitterThreadForPlayer(int id)
{
	CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(NetTransmitter), (int*)id, 0, nullptr);
}

void NetTransmitter(LPVOID sId)
{
	int id = (int)sId; //*static_cast<int*>(sId);

	while (true) 
	{
		// check if he disconnected
		/*
		if (playersSockets[id] == NULL)
		{
			printf("Player %d disconnected\n", id);
			// close the socket
			closePlayerSocket(id);
			playersCount--;
			break;
		}
		*/

		AdvanceTimerForPlayer(id);
		NetTransmit(id);
	}
}

void MessageConnectForPlayers(char playerName[255], int except)
{
	for (int playerid = 0; playerid < playersCount; playerid++)
	{
		// ignore this player because we don't want to receive our own data
		if (playerid != except)
		{
			// if that player is not null
			if (playersSockets[playerid] != NULL)
			{
				// if that player's buffer is not null
				//if (playersLastBuffer[playerid] != NULL)
				//{
					//printf("Transmitting data from player %d to player %d\n", playerid, id);
				char buf[]
				{
					COMMAND_PLAYERCONNECT,
					playerid,
					playerName[0], playerName[1], playerName[2], playerName[3],
					playerName[4], playerName[5], playerName[6], playerName[7],
					playerName[8], playerName[9], playerName[10], playerName[11],
					playerName[12], playerName[13], playerName[14], playerName[15],
					playerName[16], playerName[17], playerName[18], playerName[19],
					playerName[20], playerName[21], playerName[22], playerName[23],
					playerName[24], playerName[25], playerName[26], playerName[27],
					playerName[28], playerName[29], playerName[30], playerName[31],
					playerName[32], playerName[33], playerName[34], playerName[35],
					playerName[36], playerName[37], playerName[38], playerName[39]
				};
				send(playersSockets[playerid], /*playersLastBuffer[playerid]*/buf, 256, 0);
				//}
			}
		}
	}
}

void MessageDisconnectForPlayers(char playerName[255], int except)
{
	for (int playerid = 0; playerid < playersCount; playerid++)
	{
		// ignore this player because we don't want to receive our own data
		if (playerid != except)
		{
			// if that player is not null
			if (playersSockets[playerid] != NULL)
			{
				// if that player's buffer is not null
				//if (playersLastBuffer[playerid] != NULL)
				//{
					//printf("Transmitting data from player %d to player %d\n", playerid, id);
				char buf[]
				{
					COMMAND_PLAYERDISCONNECT,
					playerid,
					playerName[0], playerName[1], playerName[2], playerName[3],
					playerName[4], playerName[5], playerName[6], playerName[7],
					playerName[8], playerName[9], playerName[10], playerName[11],
					playerName[12], playerName[13], playerName[14], playerName[15],
					playerName[16], playerName[17], playerName[18], playerName[19],
					playerName[20], playerName[21], playerName[22], playerName[23],
					playerName[24], playerName[25], playerName[26], playerName[27],
					playerName[28], playerName[29], playerName[30], playerName[31],
					playerName[32], playerName[33], playerName[34], playerName[35],
					playerName[36], playerName[37], playerName[38], playerName[39]
				};
				send(playersSockets[playerid], /*playersLastBuffer[playerid]*/buf, 256, 0);
				//}
			}
		}
	}
}

void NetTransmit(int id)
{
	// send other players data to that player
	for (int playerid = 0; playerid < playersCount; playerid++)
	{
		// ignore this player because we don't want to receive our own data
		if (playerid != id)
		{
			// if that player is not null
		if (playersSockets[playerid] != NULL)
		{
			// if that player's buffer is not null
			//if (playersLastBuffer[playerid] != NULL)
			//{
				//printf("Transmitting data from player %d to player %d\n", playerid, id);
				char buf[256];
				ZeroMemory(buf, 256);
				int bytesReceived = recv(playersSockets[playerid], buf, 256, 0);
				if (bytesReceived != 0 && bytesReceived != -1) 
				{
					send(playersSockets[id], /*playersLastBuffer[playerid]*/buf, 256, 0);
				}
			//}
		}
		}
	}
}

void StartListeningToPlayer(int id)
{
	CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(WaitForPlayerData), (int*)id, 0, nullptr);
}

// thread created in main.cpp
void RequestAIVehicleDataLoop()
{
	while (RequestAIData) {
		RequestLastPlayerForAIVehicleData();
		Sleep(RequestAIDataSleepTime);
	}
}

void RequestLastPlayerForAIVehicleData()
{
	for (int id = 0; id < playersCount; id++)
	{
		if (playersSockets[id] != NULL)
		{
			char buf[]
			{
				COMMAND_REQUEST_AIVEHICLEDATA
			};
			send(playersSockets[id], /*playersLastBuffer[playerid]*/buf, 1, 0);
			break;
		}
	}
}

void WaitForPlayerData(LPVOID sId)
{
	int id = (int)sId; //*static_cast<int*>(sId);
	
	while (true) {
		// wait for client data
			if (playersSockets[id] != NULL) {

				char buf[256];
				ZeroMemory(buf, 256);
				int bytesReceived = recv(playersSockets[id], buf, 256, 0);
				if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
				{
					//printf("Player %d disconnected\n",id);
					const char* name = playersNames[id];
					if (name == NULL)
					{
						name = "???";
					}
					char playerNameB[255]
					{ 
						name[0], name[1], name[2], name[3],
						name[4], name[5], name[6], name[7],
						name[8], name[9], name[10], name[11],
						name[12], name[13], name[14], name[15],
						name[16], name[17], name[18], name[19],
						name[20], name[21], name[22], name[23],
						name[24], name[25], name[26], name[27],
						name[28], name[29], name[30], name[31],
						name[32], name[33], name[34], name[35],
						name[36], name[37], name[38], name[39] 
					};
					MessageDisconnectForPlayers(playerNameB, -1);
					printf("%s disconnected\n",name);
					// close the socket
					DisposePlayer(id);
					playersCount = playersCount - 1;
					break;
				}
				else 
				{
					/*
					char bDamage[4] = { buf[1], buf[2], buf[3], buf[4] };
					char bBoostTime[4] = { buf[5], buf[6], buf[7], buf[8] };

					char bX[4] = { buf[9], buf[10], buf[11], buf[12] };
					char bY[4] = { buf[13], buf[14], buf[15], buf[16] };
					char bZ[4] = { buf[17], buf[18], buf[19], buf[20] };

					char bDX[4] = { buf[21], buf[22], buf[23], buf[24] };
					char bDY[4] = { buf[25], buf[26], buf[27], buf[28] };
					char bDZ[4] = { buf[29], buf[30], buf[31], buf[32] };
					char bDW[4] = { buf[33], buf[34], buf[35], buf[36] };

					char bVX[4] = { buf[37], buf[38], buf[39], buf[40] };
					char bVY[4] = { buf[41], buf[42], buf[43], buf[44] };
					char bVZ[4] = { buf[45], buf[46], buf[47], buf[48] };
					char bVW[4] = { buf[49], buf[50], buf[51], buf[52] };
					char bSteer[4] = { buf[53], buf[54], buf[55], buf[56] };

					float damage = (*(float*)bDamage);
					float boost = (*(float*)bBoostTime);
					float steer = (*(float*)bSteer);

					float x = (*(float*)bX);
					float y = (*(float*)bY);
					float z = (*(float*)bZ);

					float dx = (*(float*)bDX);
					float dy = (*(float*)bDY);
					float dz = (*(float*)bDZ);
					float dw = (*(float*)bDW);

					float vx = (*(float*)bVX);
					float vy = (*(float*)bVY);
					float vz = (*(float*)bVZ);
					float vw = (*(float*)bVW);
					*/

					// debug
					//printf("Player %d has sent data\n",id);
					//printf("Player %d is using truck %d and is positioned at \n%f, %f, %f\n%f, %f, %f, %f\nWithin damage and boost %f and %f\nAnd with the velocity\n%f, %f, %f, %f\nSteer input of %f\n", id, (int)buf[0], x,y,z, dx,dy,dz,dw, damage, boost, vx, vy, vz, vw, steer);
					
					// do something about it
					//playersLastBuffer[id] = buf;
					
					//AdvanceTimerForPlayer(id);
					NetTransmit(id);

					/*
					printf("Player %d data:\n", id, (int)buf[0], x,y,z);
					for (int bid = 0; bid < bytesReceived; bid++)
					{
						printf("%X ", buf[bid]);
					}
					printf("\n");
				    */
				}
			}
	}

	// shut down winsock
	//WSACleanup();
}

void StartUpdate()
{
	while (true)
	{
		UpdateServer();
		Sleep(1);
	}
}

void UpdateServer()
{
	// TODO: Update server here
}

void SetRequestAIData(bool value)
{
	RequestAIData = value;
}
void SetRequestAIDataSleepTime(int value)
{
	RequestAIDataSleepTime = value;
}