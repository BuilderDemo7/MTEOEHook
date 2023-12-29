#include <iostream>
#include "client.h"
#include "CVehicle.h"
#include <WS2tcpip.h>
#include "DDIAppClass.h" // input stuff

#pragma comment (lib, "ws2_32.lib")

void float2binary(float f, char* output)
{
	//char result[sizeof(float)];
	memcpy(output, &f, sizeof(f));
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
		StartNetSend();
	}

	return connected;
}

// emulate other players on this client
void NetRecord()
{
	static int sus = 9000;
	while (connected)
	{
		char buf[4096];
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived != SOCKET_ERROR)
		{
			Truck vehicle = (Truck)( (int)buf[0] );

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

			char bAVX[4] = { buf[57], buf[58], buf[59], buf[60] };
			char bAVY[4] = { buf[61], buf[62], buf[63], buf[64] };
			char bAVZ[4] = { buf[65], buf[66], buf[67], buf[68] };
			char bAVW[4] = { buf[69], buf[70], buf[71], buf[72] };

			char bThrottle[4] = { buf[73], buf[74], buf[75], buf[76] };

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

			if (vehicle != TRUCK_INVALID && vehicle < 8)
			{
				CVehicle* veh = GetVehicleOfType(vehicle);
				if (veh != NULL)
				{
					DoNotResetControlsForVehicle(veh);
					veh->SetControllingFlags(0); // this will disable AI for that vehicle
					// checks if the position is not too high
					// (when impostor is sus)
					if (x<sus && z<sus && x>-sus && z>-sus)
					{
						// set position if not sus
						veh->SetFacingDirection(Vector4(dx, dy, dz, dw)); // dw is important for roll btw ig
						Vector4* pos = veh->GetPosition();
						//veh->SetPosition(Vector4(x, y, z, 0));
						pos->Lerp(Vector4(x, y, z, 0),0.5f);
						// set velocity if not sus
						if (vx < sus && vz < sus && vx > -sus && vz > -sus) 
						{
							veh->SetVelocity(Vector4(vx, vy, vz, vw));
							//veh->SetAngularVelocity(Vector4(avx, avy, avz, avw));
						}
						printf("Recording player position at %f, %f, %f\nVelocity of %f, %f, %f\nAngular Velocity of %f, %f, %f, %f\n", x, y, z, vx, vy, vz, avx, avy, avz, avw);
					}
					veh->SetBoostingTime(boost);
					veh->SetDamage(damage);

					veh->SetSteer(steer);
					veh->SetThrottle(th);
				}
			}
			Sleep(RecordSleepTime);
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

void NetSend()
{
	// send player data to the server
	CVehicle* playerVeh = GetPlayerVehicle();
	if (playerVeh != NULL && playerVeh != 0 && playerVeh != nullptr )
	{
		try
		{
			Vector4* pos = playerVeh->GetPosition();
			Vector4* dir = playerVeh->GetFacingDirection();
			Vector4* vel = playerVeh->GetVelocity();
			Vector4* avel = playerVeh->GetVelocity();
			float damage = playerVeh->GetDamage();
			float boost = playerVeh->GetBoostingTime();
			float steer = playerVeh->GetSteer();
			float th = playerVeh->GetThrottle();

			// other info
			char bDamage[sizeof(float)];
			char bBoostTime[sizeof(float)];
			char bSteer[sizeof(float)];
			char bThrottle[sizeof(float)];

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

			// hey sorry for my hardcoded crap lol
			char buffer[]
			{
				// referenced truck
				(int)GetSelectedTruck(),

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
			};
			send(clientSocket, buffer, 4096, 0);
		}
		catch (char *e)
		{
			// we failed
		}
	}
}