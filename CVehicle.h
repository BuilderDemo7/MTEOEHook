#pragma once
#include <iostream>
#include "core/Vector4.h"

#define MAXIMUM_VEHICLES_TOGET 64

// note: in original order from the game config files
enum Truck : int
{
	TRUCK_INVALID = -1,

	TRUCK_DOOM = 0,
	TRUCK_FLAME = 1,
	TRUCK_PRO = 2,
	TRUCK_WRENCH = 3,
	TRUCK_CRUSH = 4,
	TRUCK_MIDNIGHT = 5,
	TRUCK_DUMPER = 6,
	TRUCK_BY4 = 7
};

class CVehicle
{
public:
	void SetDamage(float damage);
	float GetDamage();

	int GetNitroLevel();
	void SetNitroLevel(int nitroLevel);

	void SetControllingFlags(int flags);
	int GetControllingFlags();

	int GetGear();
	int GetVehicleDataPointer();

	// racing
	float GetCurrentLapTime();
	void SetCurrentLapTime(float time);

	void SetBestLapTime(float time);
	float GetBestLapTime();

	float GetWrongWayDistance();
	void SetWrongWayDistance(float distance);

	int GetPlace();
	void SetPlace(int th);

	int GetLap();
	void SetLap(int lap);

	// boost/nitro
	float GetBoostingTime();
	void SetBoostingTime(float time);

	void TriggerNitro();

	// matrix
	Vector4* GetPosition();
	void SetPosition(Vector4 position);

	Vector4* GetFacingDirection();
	void SetFacingDirection(Vector4 direction);

	// physics
	Vector4* GetVelocity();
	void SetVelocity(Vector4 velocity);
	
	Vector4* GetAngularVelocity();
	void SetAngularVelocity(Vector4 velocity);

	// input
	void SetThrottle(float throttle);
	float GetThrottle();

	void SetSteer(float steer);
	float GetSteer();

	void SetHandbrake(bool handbrake);
	bool GetHandbrake();

	// misc
	Truck GetVehicleType();
};

void VehicleGet();
void GetPlayerSelectedVehIdHook();
static Truck selectedTruck = TRUCK_INVALID;
CVehicle* GetLastVehicleGet();

Truck GetSelectedTruck();

CVehicle* GetVehicleOfType(Truck type);
CVehicle* GetPlayerVehicle();

static bool VehicleCheats[2] = { false }; // cheats from this hook
void UpdateVehicleCheats();
void ToggleVehicleCheat(int id, bool state);

const char* GetVehicleNameFromType(Truck type);
static CVehicle* LastVehicleGet = NULL;
static int LastVehicleGetId = 0;
static int maximumVehicleGets = MAXIMUM_VEHICLES_TOGET;
static CVehicle* LastVehicleGets[(int)MAXIMUM_VEHICLES_TOGET] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

