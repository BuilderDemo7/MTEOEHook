#include <iostream>
#include "client.h"
#include <WS2tcpip.h>
#include "DDIAppClass.h" // input stuff

// shared variables between other programs
#define SHARED_VARIABLES_MULTIPLAYER_CONNECTED 0x04A00100
#define SHARED_VARIABLES_MULTIPLAYER_PLAYERSCOUNT 0x08A00100

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

void float2binary(float f, char* output)
{
	//char result[sizeof(float)];
	memcpy(output, &f, sizeof(f));
}
void int2binary(int i, char* output)
{
	//char result[sizeof(float)];
	memcpy(output, &i, sizeof(i));
}

bool IsConnected()
{
	return connected;
}

bool ConnectTo(short port, const char* ip)
{
	// initialize socket
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		printf("Something went wrong in initializing winsock!\n");
		return false;
	}

	// create socket and connect
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(hint.sin_addr));

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("Can't create client socket!\n");
		return false;
	}
	int connectCode = connect(clientSocket, (sockaddr*)&hint, sizeof(hint) );
	printf("Connection returned the code %d\n",connectCode);
	connected = connectCode == -1 ? false : true;

	if (connected) {
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(NetRecord), nullptr, 0, nullptr);;
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(SubNetRecord), nullptr, 0, nullptr);; // for AI
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(NetRecord_TimerStart), nullptr, 0, nullptr);;
		StartNetSend();
	}

	return connected;
}

void NetRecord_TimerStart()
{
	while (connected) {
		LastReceiveTime += 0.03;
	}
}

void RecordVehicleData(char buf[256], bool disableAI = true)
{
	static int sus = 10000;

	int dataOffset = 1; // I may change this later
	Truck vehicle = (Truck)((int)buf[0 + dataOffset]);

	char bDamage[4] = { buf[1 + dataOffset], buf[2 + dataOffset], buf[3 + dataOffset], buf[4 + dataOffset] };
	char bBoostTime[4] = { buf[5 + dataOffset], buf[6 + dataOffset], buf[7 + dataOffset], buf[8 + dataOffset] };

	char bX[4] = { buf[9 + dataOffset], buf[10 + dataOffset], buf[11 + dataOffset], buf[12 + dataOffset] };
	char bY[4] = { buf[13 + dataOffset], buf[14 + dataOffset], buf[15 + dataOffset], buf[16 + dataOffset] };
	char bZ[4] = { buf[17 + dataOffset], buf[18 + dataOffset], buf[19 + dataOffset], buf[20 + dataOffset] };

	char bDX[4] = { buf[21 + dataOffset], buf[22 + dataOffset], buf[23 + dataOffset], buf[24 + dataOffset] };
	char bDY[4] = { buf[25 + dataOffset], buf[26 + dataOffset], buf[27 + dataOffset], buf[28 + dataOffset] };
	char bDZ[4] = { buf[29 + dataOffset], buf[30 + dataOffset], buf[31 + dataOffset], buf[32 + dataOffset] };
	char bDW[4] = { buf[33 + dataOffset], buf[34 + dataOffset], buf[35 + dataOffset], buf[36 + dataOffset] };

	char bVX[4] = { buf[37 + dataOffset], buf[38 + dataOffset], buf[39 + dataOffset], buf[40 + dataOffset] };
	char bVY[4] = { buf[41 + dataOffset], buf[42 + dataOffset], buf[43 + dataOffset], buf[44 + dataOffset] };
	char bVZ[4] = { buf[45 + dataOffset], buf[46 + dataOffset], buf[47 + dataOffset], buf[48 + dataOffset] };
	char bVW[4] = { buf[49 + dataOffset], buf[50 + dataOffset], buf[51 + dataOffset], buf[52 + dataOffset] };

	char bSteer[4] = { buf[53 + dataOffset], buf[54 + dataOffset], buf[55 + dataOffset], buf[56 + dataOffset] };

	char bAVX[4] = { buf[57 + dataOffset], buf[58 + dataOffset], buf[59 + dataOffset], buf[60 + dataOffset] };
	char bAVY[4] = { buf[61 + dataOffset], buf[62 + dataOffset], buf[63 + dataOffset], buf[64 + dataOffset] };
	char bAVZ[4] = { buf[65 + dataOffset], buf[66 + dataOffset], buf[67 + dataOffset], buf[68 + dataOffset] };
	char bAVW[4] = { buf[69 + dataOffset], buf[70 + dataOffset], buf[71 + dataOffset], buf[72 + dataOffset] };

	char bThrottle[4] = { buf[73 + dataOffset], buf[74 + dataOffset], buf[75 + dataOffset], buf[76 + dataOffset] };

	char bDT[4] = { buf[77 + dataOffset], buf[78 + dataOffset], buf[79 + dataOffset], buf[80 + dataOffset] };

	char bLap[4] = { buf[81 + dataOffset], buf[82 + dataOffset], buf[83 + dataOffset], buf[84 + dataOffset] };

	float damage = (*(float*)bDamage);
	float boost = (*(float*)bBoostTime);
	float steer = (*(float*)bSteer);
	float th = (*(float*)bThrottle);

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

	float avx = (*(float*)bAVX);
	float avy = (*(float*)bAVY);
	float avz = (*(float*)bAVZ);
	float avw = (*(float*)bAVW);

	float dt = (*(float*)bDT);

	int lap = (*(int*)bLap);

	if (vehicle != TRUCK_INVALID && vehicle < 8)
	{
		CVehicle* veh = GetVehicleOfType(vehicle);
		if (veh != NULL && veh != 0 && vehicle != GetSelectedTruck())
		{
			if (disableAI) {
				DoNotResetControlsForVehicle(veh);
				veh->SetControllingFlags(0); // this will disable AI for that vehicle
			}
			// checks if the position is not too high
			// (when impostor is sus)
			printf("Confirming vehicle at: X %f, Y %f, Z %f\n", x, y, z);
			if (x<sus && z<sus && x>-sus && z>-sus)
			{
				// set position if not sus
				veh->SetFacingDirection(Vector4(dx, dy, dz, dw)); // the dw is important for roll btw ig
				//Vector4* pos = veh->GetPosition();
				veh->SetPosition(Vector4(x, y, z, 0));
				//pos->Lerp(Vector4(x, y, z, 0),0.5f);
				//veh->SetPosition(Vector4(pos->X, pos->Y, pos->Z, pos->W));
				// set velocity if not sus
				float cdt = GetDeltaTime();
				float delta = (dt + cdt)+1;
				printf("-> Delta = %f\n", delta);
				if (vx < sus && vz < sus && vx > -sus && vz > -sus)
				{
					veh->SetVelocity(Vector4(vx / delta, vy / delta, vz / delta, vw));
					veh->SetAngularVelocity(Vector4(avx, avy, avz, avw));
					if (lap >= 0 && lap <= 3) {
						veh->SetLap(lap);
					}
				}
				printf("Recording vehicle position at %f, %f, %f\nVelocity of %f, %f, %f\nAngular Velocity of %f, %f, %f, %f\n", x, y, z, vx, vy, vz, avx, avy, avz, avw);
			}
			veh->SetBoostingTime(boost);
			veh->SetDamage(damage);

			veh->SetSteer(steer);
			veh->SetThrottle(th);
		}
	}
}

// emulate AI on this client in a seperate thread preventing connection problems/slowness
void SubNetRecord()
{
	while (connected)
	{
		char buf[256];
		ZeroMemory(buf, 256);
		int bytesReceived = recv(clientSocket, buf, 256, 0);
		if (bytesReceived != SOCKET_ERROR)
		{
			int command = (int)buf[0];
			if (command == COMMAND_AIVEHICLEDATA)
			{
				if (!serverAskedMEForAIVehicleData) {
					RecordVehicleData(buf, false);
					//Sleep(RecordSleepTime);
				}
			}
			else if (command == COMMAND_REQUEST_AIVEHICLEDATA)
			{
				serverAskedMEForAIVehicleData = true;
				SendAIVehicleDatas();
			}
		}
	}
}

// emulate other players on this client
void NetRecord()
{
	while (connected)
	{
		char buf[256];
		ZeroMemory(buf, 256);
		int bytesReceived = recv(clientSocket, buf, 256, 0);
		if (bytesReceived != SOCKET_ERROR)
		{
			int command = (int)buf[0];
			//printf("-> Main command received in progress >> %x\n", command);
			if (command == COMMAND_PLAYERVEHICLEDATA)
			{
				RecordVehicleData(buf, true);
				//Sleep(RecordSleepTime);
			}
			/*
			else if (command == COMMAND_AIVEHICLEDATA)
			{
				if (!serverAskedMEForAIVehicleData) {
					RecordVehicleData(buf, false);
					//Sleep(RecordSleepTime);
				}
			}
			else if (command == COMMAND_REQUEST_AIVEHICLEDATA)
			{
				serverAskedMEForAIVehicleData = true;
		        SendAIVehicleDatas();
			}
			*/
			else if (command == COMMAND_PLAYERCONNECT)
			{
				int id = (int)buf[1];
				char playerName[]
				{
					buf[2], buf[3],
					buf[4], buf[5], buf[6], buf[7],
					buf[8], buf[9], buf[10], buf[11],
					buf[12], buf[13], buf[14], buf[15],
					buf[16], buf[17], buf[18], buf[19],
					buf[20], buf[21], buf[22], buf[23],
					buf[24], buf[25], buf[26], buf[27],
					buf[28], buf[29], buf[30], buf[31],
					buf[32], buf[33], buf[34], buf[35],
					buf[36], buf[37], buf[38], buf[39],
					buf[40], buf[41]
				};
				PlayersCount++;
				printf("Player %s (%d) connected\n", playerName, id);
			}
			else if (command == COMMAND_PLAYERDISCONNECT)
			{
			int id = (int)buf[1];
			char playerName[]
			{
				buf[2], buf[3],
				buf[4], buf[5], buf[5], buf[6],
				buf[7], buf[8], buf[9], buf[10],
				buf[11], buf[12], buf[13], buf[14],
				buf[15], buf[16], buf[17], buf[18],
				buf[19], buf[20], buf[21], buf[22],
				buf[23], buf[24], buf[25], buf[26],
				buf[27], buf[28], buf[29], buf[30],
				buf[31], buf[32], buf[33], buf[34],
				buf[35], buf[36], buf[37], buf[38],
			};
			PlayersCount--;
			printf("Player %s (%d) disconnected\n", playerName, id);
			}
			/*
			else if (command == COMMAND_TIMEADVANCE)
			{
			    CVehicle* playerVeh = GetPlayerVehicle();
			    if (playerVeh != NULL && playerVeh != 0 && playerVeh != nullptr)
		    	{
					float time = playerVeh->GetCurrentLapTime();
					playerVeh->SetCurrentLapTime(time + LastReceiveTime + GetDeltaTime());
			    }
            }
			*/
			LastReceiveTime = 0;
		}
		else
		{
			printf("Connection lost.\n");
			connected = false;
			WSACleanup();
			break;
		}
	}
}


void StartNetSend()
{
	CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(StartNetSendLoop), nullptr, 0, nullptr);
}

void StartNetSendLoop()
{
	while (connected)
	{
		NetSend();
		Sleep(1);
	}
}

SOCKET GetClientSocket()
{
	return clientSocket;
}

void SendVehicleData(CVehicle* playerVeh, bool AI = false)
{
	Vector4* pos = playerVeh->GetPosition();
	Vector4* dir = playerVeh->GetFacingDirection();
	Vector4* vel = playerVeh->GetVelocity();
	Vector4* avel = playerVeh->GetAngularVelocity();
	float damage = playerVeh->GetDamage();
	float boost = playerVeh->GetBoostingTime();
	float steer = playerVeh->GetSteer();
	float th = playerVeh->GetThrottle();
	float dt = GetDeltaTime();
	int lap = playerVeh->GetLap();

	// other info
	char bDamage[sizeof(float)];
	char bBoostTime[sizeof(float)];
	char bSteer[sizeof(float)];
	char bThrottle[sizeof(float)];

	char bDT[sizeof(float)]; // delta time in binary

	char bLap[sizeof(float)]; // lap in bin

	// position
	char bPX[sizeof(float)];
	char bPY[sizeof(float)];
	char bPZ[sizeof(float)];

	// velocity
	char bVX[sizeof(float)];
	char bVY[sizeof(float)];
	char bVZ[sizeof(float)];
	char bVW[sizeof(float)];

	// a. velocity
	char bAVX[sizeof(float)];
	char bAVY[sizeof(float)];
	char bAVZ[sizeof(float)];
	char bAVW[sizeof(float)];

	// direction
	char bDX[sizeof(float)];
	char bDY[sizeof(float)];
	char bDZ[sizeof(float)];
	char bDW[sizeof(float)];

	float2binary(pos->X, bPX);
	float2binary(pos->Y, bPY);
	float2binary(pos->Z, bPZ);

	float2binary(dir->X, bDX);
	float2binary(dir->Y, bDY);
	float2binary(dir->Z, bDZ);
	float2binary(dir->W, bDW);

	float2binary(vel->X, bVX);
	float2binary(vel->Y, bVY);
	float2binary(vel->Z, bVZ);
	float2binary(vel->W, bVW);

	float2binary(avel->X, bAVX);
	float2binary(avel->Y, bAVY);
	float2binary(avel->Z, bAVZ);
	float2binary(avel->W, bAVW);

	float2binary(damage, bDamage);
	float2binary(boost, bBoostTime);
	float2binary(steer, bSteer);
	float2binary(th, bThrottle);

	float2binary(dt, bDT);

	int2binary(lap, bLap);

	// hey sorry for my hardcoded crap lol
	int com = AI ? COMMAND_AIVEHICLEDATA : COMMAND_PLAYERVEHICLEDATA;
	int vehType = (int)playerVeh->GetVehicleType();
	char buffer[]
	{
		// command
		com,

		// referenced truck
		(int)vehType,

		// other info
		bDamage[0], bDamage[1], bDamage[2], bDamage[3],
		bBoostTime[0], bBoostTime[1], bBoostTime[2], bBoostTime[3],

		// position
		bPX[0], bPX[1], bPX[2], bPX[3],
		bPY[0], bPY[1], bPY[2], bPY[3],
		bPZ[0], bPZ[1], bPZ[2], bPZ[3],
		// facing direction
		bDX[0], bDX[1], bDX[2], bDX[3],
		bDY[0], bDY[1], bDY[2], bDY[3],
		bDZ[0], bDZ[1], bDZ[2], bDZ[3],
		bDW[0], bDW[1], bDW[2], bDW[3],
		// velocity
		bVX[0], bVX[1], bVX[2], bVX[3],
		bVY[0], bVY[1], bVY[2], bVY[3],
		bVZ[0], bVZ[1], bVZ[2], bVZ[3],
		bVW[0], bVW[1], bVW[2], bVW[3],
		// input
		bSteer[0], bSteer[1], bSteer[2], bSteer[3],
		// angular velocity
		bAVX[0], bAVX[1], bAVX[2], bAVX[3],
		bAVY[0], bAVY[1], bAVY[2], bAVY[3],
		bAVZ[0], bAVZ[1], bAVZ[2], bAVZ[3],
		bAVW[0], bAVW[1], bAVW[2], bAVW[3],
		// input also
		bThrottle[0], bThrottle[1], bThrottle[2], bThrottle[3],
		// delta time
		bDT[0], bDT[1], bDT[2], bDT[3],
		// lap
		bLap[0], bLap[1], bLap[2], bLap[3]
	};
	send(clientSocket, buffer, 256, 0);
}

void SendAIVehicleDatas()
{
	for (int id = 0; id < TRUCKS_COUNT; id++) {
		CVehicle* aiVeh = GetVehicleOfType((Truck)id);
		if ((Truck)id != GetSelectedTruck()) 
		{
			if (aiVeh != NULL && aiVeh != 0 && aiVeh != nullptr) {
				SendVehicleData(aiVeh, true);
			}
		}
	}
}

void NetSend()
{
	// write shared vars
	//*(int*)(SHARED_VARIABLES_MULTIPLAYER_CONNECTED) = connected == true ? 1 : 0;
	//*(int*)(SHARED_VARIABLES_MULTIPLAYER_PLAYERSCOUNT) = PlayersCount;

	// send player data to the server
	CVehicle* playerVeh = GetPlayerVehicle();
	if (playerVeh != NULL && playerVeh != 0 && playerVeh != nullptr)
	{
			try
			{
				SendVehicleData(playerVeh,false);
			}
			catch (char *e)
			{
				// we failed
			}
	}
}