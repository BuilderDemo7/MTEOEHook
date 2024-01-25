#include <iostream>
#include "CVehicle.h"
#include "core/Vector4.h"
//#include "DDIAppClass.h"

// multiplayer
//#include "client.h"

// cheat IDs
#define CHEAT_PLAYER_INVENCIBLE_ID 0
#define CHEAT_PLAYER_INFBOOST_ID 1

// shared variables between other programs
#define SHARED_VARIABLES_PLAYER_VEHICLE 0x00A00100

/*
#define MATRIX_MAINPTR_OFFSET = 0x1F4
#define MATRIX_PTR1_OFFSET = 0x10
#define MATRIX_PTR2_OFFSET = 0x4
*/

#define VEHICLE_MAXIMUM_WRONGWAY_DISTANCE 2.0f

// bars
float CVehicle::GetDamage()
{
	return *(float*)(this + 0x508);
}

void CVehicle::SetDamage(float damage)
{
	*(float*)(this + 0x508) = damage;
}

int CVehicle::GetNitroLevel()
{
	return *(int*)(this + 0x210);
}

void CVehicle::SetNitroLevel(int nitroLevel)
{
	*(float*)(this + 0x210) = nitroLevel;
}

void CVehicle::SetControllingFlags(int flags)
{
	*(int*)(this + 0) = flags;
}

int CVehicle::GetControllingFlags()
{
	return *(int*)(this + 0);
}

int CVehicle::GetGear()
{
	return *(int*)(this + 292);
}

int CVehicle::GetVehicleDataPointer()
{
	try 
	{
		return *(int*)(this + 4);
	}
	catch (char *e)
	{
		return NULL;
	}
	return NULL;
}

/*
bool CVehicle::operator==(int ptr)
{
	if ((int)this == ptr)
		return true;
	return false;
}

bool CVehicle::operator!=(int ptr)
{
	return !((int)this == ptr);
}

bool CVehicle::operator==(unsigned int ptr)
{
	if ((int)this == ptr)
		return true;
	return false;
}

bool CVehicle::operator!=(unsigned int ptr)
{
	return !((int)this == ptr);
}
*/

// racing
float CVehicle::GetCurrentLapTime()
{
	return *(float*)(this + 0x27C);
}

void CVehicle::SetCurrentLapTime(float time)
{
	*(float*)(this + 0x27C) = time;
}

float CVehicle::GetBestLapTime()
{
	return *(float*)(this + 0x280);
}

void CVehicle::SetBestLapTime(float time)
{
	*(float*)(this + 0x280) = time;
}

// okay this one is not quite useful, or is it?
float CVehicle::GetWrongWayDistance()
{
	return *(float*)(this + 0x260);
}

void CVehicle::SetWrongWayDistance(float distance)
{
	*(float*)(this + 0x260) = distance;
}

// 1st, 2nd, etc.
int CVehicle::GetPlace()
{
	return *(int*)(this + 0x2E8);
}

void CVehicle::SetPlace(int th)
{
	*(int*)(this + 0x2E8) = th;
}

int CVehicle::GetLap()
{
	return *(int*)(this + 0x268);
}

void CVehicle::SetLap(int lap)
{
	*(int*)(this + 0x268) = lap;
}

// speed
// I found out this one while boosting and using nitro, makes a cool boosting effect but doesn't seems to boost my truck...
float CVehicle::GetBoostingTime()
{
	return *(float*)(this + 0x214);
}

void CVehicle::SetBoostingTime(float time)
{
	*(float*)(this + 0x214) = time;
}

void CVehicle::TriggerNitro()
{
	//static int funcAddr = 0x00597CF2;
	static float speedMultiplier = 2;
	this->SetBoostingTime(0.01f);
	// multiply velocity
	Vector4* vel = this->GetVelocity();
	Vector4 multiVel = Vector4(vel->X * speedMultiplier, vel->Y * speedMultiplier, vel->Z * speedMultiplier);
	this->SetVelocity(multiVel);
	/*
	_asm
	{
		mov eax, this
		jmp funcAddr
	}
	*/
}

// matrix stuff
Vector4* CVehicle::GetPosition()
{
	// at least not confusing
	int* pt1 = (int*)(this + 0x1F4);
	int* pt2 = (int*)(*pt1 + 0x10);
	int* pt3 = (int*)(*pt2 + 0x4);

	int* pt4 = (int*)(*pt3);

	return (Vector4*)(*pt4 + 0x30);
}

void CVehicle::SetPosition(Vector4 position)
{
	// at least not confusing
	int* pt1 = (int*)(this + 0x1F4);
	int* pt2 = (int*)(*pt1 + 0x10);
	int* pt3 = (int*)(*pt2 + 0x4);

	int* pt4 = (int*)(*pt3);

	*(Vector4*)(*pt4 + 0x30) = position;
}

// couldn't find vectors who look like forward, left and up so this might do the trick
// and W seems to be the roll value somehow
Vector4* CVehicle::GetFacingDirection()
{
	// at least not confusing
	int* pt1 = (int*)(this + 0x1F4);
	int* pt2 = (int*)(*pt1 + 0x10);
	int* pt3 = (int*)(*pt2 + 0x4);

	int* pt4 = (int*)(*pt3);

	return (Vector4*)(*pt4 + 0x40);
}

void CVehicle::SetFacingDirection(Vector4 direction)
{
	// at least not confusing
	int* pt1 = (int*)(this + 0x1F4);
	int* pt2 = (int*)(*pt1 + 0x10);
	int* pt3 = (int*)(*pt2 + 0x4);

	int* pt4 = (int*)(*pt3);

	*(Vector4*)(*pt4 + 0x40) = direction;
}

// physics
Vector4* CVehicle::GetVelocity()
{
	// at least not confusing
	int* pt1 = (int*)(this + 0x1F4);
	int* pt2 = (int*)(*pt1 + 0x10);
	int* pt3 = (int*)(*pt2 + 0x4);

	int* pt4 = (int*)(*pt3);

	return (Vector4*)(*pt4 + 0x54);
}

void CVehicle::SetVelocity(Vector4 velocity)
{
	// at least not confusing
	int* pt1 = (int*)(this + 0x1F4);
	int* pt2 = (int*)(*pt1 + 0x10);
	int* pt3 = (int*)(*pt2 + 0x4);

	int* pt4 = (int*)(*pt3);

	*(Vector4*)(*pt4 + 0x54) = velocity;
}

Vector4* CVehicle::GetAngularVelocity()
{
	// at least not confusing
	int* pt1 = (int*)(this + 0x1F4);
	int* pt2 = (int*)(*pt1 + 0x10);
	int* pt3 = (int*)(*pt2 + 0x4);

	int* pt4 = (int*)(*pt3);

	return (Vector4*)(*pt4 + 0x64);
}

void CVehicle::SetAngularVelocity(Vector4 velocity)
{
	// at least not confusing
	int* pt1 = (int*)(this + 0x1F4);
	int* pt2 = (int*)(*pt1 + 0x10);
	int* pt3 = (int*)(*pt2 + 0x4);

	int* pt4 = (int*)(*pt3);

	*(Vector4*)(*pt4 + 0x64) = velocity;
}

// input
float CVehicle::GetThrottle()
{
	return *(float*)(this + 0xA8);
}

void CVehicle::SetThrottle(float throttle)
{
	*(float*)(this + 0xA8) = throttle;
}

CVehicle* GetLastVehicleGet()
{
	return LastVehicleGet;
}

float CVehicle::GetSteer()
{
	return *(float*)(this + 0xA4);
}

void CVehicle::SetSteer(float steer)
{
	*(float*)(this + 0xA4) = steer;
}

bool CVehicle::GetHandbrake()
{
	return *(bool*)(this + 0xA8);
}

void CVehicle::SetHandbrake(bool handbrake)
{
	*(bool*)(this + 0xA8) = handbrake;
}

// misc
Truck CVehicle::GetVehicleType()
{
	try {
		if (this == nullptr)
		{
			return TRUCK_INVALID; // nuh-uh, this is a nullptr. screw this I'm returning a invalid enum to ya!
		}
		int dataPtr = 0; 
		try 
		{
			dataPtr = this->GetVehicleDataPointer();
		}
		catch (char *e)
		{
			return TRUCK_INVALID;
		}
	    if (dataPtr <= 0 || dataPtr == NULL)
		{
			return TRUCK_INVALID; // pointer to the vehicle data is not valid so
		}

		char* entry = (char*)(dataPtr + 4);
		try {
			if ((int)entry == 0xcccccccc)
			{
				return TRUCK_INVALID;
			}
			if (entry[0] == 'W' && entry[1] == 'R')
			{
				return TRUCK_WRENCH;
			}
			if (entry[0] == 'P' && entry[1] == 'R')
			{
				return TRUCK_PRO;
			}
			if (entry[0] == 'F' && entry[1] == 'L')
			{
				return TRUCK_FLAME;
			}
			if (entry[0] == 'B' && entry[1] == 'Y')
			{
				return TRUCK_BY4;
			}
			if (entry[0] == 'D' && entry[1] == 'O')
			{
				return TRUCK_DOOM;
			}
			if (entry[0] == 'C' && entry[1] == 'R')
			{
				return TRUCK_CRUSH;
			}
			if (entry[0] == 'M' && entry[1] == 'I')
			{
				return TRUCK_MIDNIGHT;
			}
			if (entry[0] == 'D' && entry[1] == 'U')
			{
				return TRUCK_DUMPER;
			}
		}
		catch (char* ex)
		{
			return TRUCK_INVALID;
		}
	}
	catch (char *e)
	{
		// there's nothing we can do when access violation comes...
	}
	// return invalid type otherwise not found
	return TRUCK_INVALID;
}

const char* GetVehicleNameFromType(Truck type)
{
	if (type == TRUCK_DUMPER)
		return "DUMPER";
	else if (type == TRUCK_CRUSH)
		return "CRUSH";
	else if (type == TRUCK_MIDNIGHT)
		return "MIDNIGHT";
	else if (type == TRUCK_DOOM)
		return "DOOM";
	else if (type == TRUCK_BY4)
		return "BY-4";
	else if (type == TRUCK_PRO)
		return "PRO";
	else if (type == TRUCK_FLAME)
		return "FLAME";
	else if (type == TRUCK_WRENCH)
		return "WRENCH";
	// return invalid type otherwise not found
	return "INVALID";
}

CVehicle* GetVehicleOfType(Truck type)
{
	for (int id = 0; id < maximumVehicleGets; id++)
	{
		CVehicle* veh = LastVehicleGets[id];
		if (veh != NULL) 
		{
			//printf("-> %s (%d)\n", GetVehicleNameFromType(veh->GetVehicleType()), id );
			if (veh->GetVehicleType() == type)
				return veh;
		}
		else
		{
			//printf("-> NULL (%d)\n",id);
		}
	}
	return NULL;
}

Truck GetSelectedTruck()
{
	return selectedTruck;
}

CVehicle* GetPlayerVehicle()
{
	if (selectedTruck != TRUCK_INVALID)
	{
		try 
		{
			CVehicle* criteriaTruck = NULL; 
			try
			{
				criteriaTruck = GetVehicleOfType(selectedTruck);
			}
			catch (char *e)
			{
				return NULL;
			}
		    if (criteriaTruck != NULL)
			{
				if (criteriaTruck->GetVehicleType() != TRUCK_INVALID) {
					//*(int*)(SHARED_VARIABLES_PLAYER_VEHICLE) = (int)criteriaTruck;
					return criteriaTruck;
				}
			}
		}
		catch(char *e)
		{
			return NULL;
		}
	}
	return NULL;
}

// hook
void UpdateVehicle(CVehicle* vehicle)
{
	UpdateVehicleCheats();
	//if (IsConnected())
	//{
	//	NetSend();
	//}
}

void _declspec(naked) VehicleGet()
{
	if (LastVehicleGetId >= maximumVehicleGets)
		LastVehicleGetId = 0;

	static int jumpcontinue = 0x0058F4DF;
	static int calladdr = 0x0046C7D0;
	static int pVeh;
	//int pVehicle = 0;
	// hook in
	_asm {
		mov eax, [esi + 04]
		mov[pVeh], esi
		push eax
		call calladdr
	}
	//printf("-> %p\n",pVeh);
	LastVehicleGet = (CVehicle*)pVeh;
	UpdateVehicle(LastVehicleGet);
	LastVehicleGets[LastVehicleGetId] = LastVehicleGet;
	LastVehicleGetId++;
	// jump back
	_asm {
		jmp jumpcontinue
	}
}

void _declspec(naked) GetPlayerSelectedVehIdHook()
{
	static int jump_continue = 0x0040959F;
	static int jmp_1 = 0x004095A1;
	static int vehIdptr = 0;
	// main code
	_asm 
	{
		mov eax, [ecx + edx]
		mov[vehIdptr], edx
	}
	selectedTruck = (Truck) ( *(int*)(vehIdptr) );
	// rest of the code
	_asm 
	{
		jmp jmp_1 //"Monster Trux Extreme - Offroad Edition.exe" + 95A1

		// I'm not sure if this instruction will ever be accessed
		//jmp jump_continue
	}
}

// cheats

void ToggleVehicleCheat(int id, bool state = NULL)
{
	if (state == NULL)
		state = !VehicleCheats[id];

	VehicleCheats[id] = state;
}

void UpdateVehicleCheats()
{
	if (VehicleCheats[CHEAT_PLAYER_INVENCIBLE_ID] == true)
	{
		CVehicle* playerVeh = GetPlayerVehicle();
		if (playerVeh != NULL & playerVeh != 0)
		{
			playerVeh->SetDamage(9999);
		}
	}
	if (VehicleCheats[CHEAT_PLAYER_INFBOOST_ID] == true)
	{
		CVehicle* playerVeh = GetPlayerVehicle();
		if (playerVeh != NULL & playerVeh != 0)
		{
			playerVeh->SetBoostingTime(0.2f);
		}
	}
}