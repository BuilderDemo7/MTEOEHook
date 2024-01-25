#pragma once
#include "CVehicle.h"
#include "client.h"
#include <iostream>
#include <Windows.h>


typedef struct
{
	int CustomInput = 0; // bool

	int AccelerateKey = VK_RETURN; // 13
	int BrakeKey = VK_RSHIFT; // 161

	int LeftKey = VK_LEFT; // 37
	int RightKey = VK_RIGHT; // 39
} Input;

class DDIAppClass
{
public:
	static void Kill(int exitCode);
	static void Unknown();
};

void DoNotResetControlsForVehicle(CVehicle* vehicle);
void ToggleDontResetControls(bool state);
void OnGameUpdateHook();
void InputUpdate();

static bool InGame = false; // if is in a race or not
static float playerSteer = 0.0f;

static bool DontResetControls = false;
static bool DontResetThrottle = false;
static CVehicle* DontResetControlsForVehicle;

static Input InputConfig;
void SetInputConfig(Input cfg);

void DisableObeyAccelerateInputHook();
void DisableObeyBrakeInputHook();
void SetSteerAxisHook();
void DisableSteerLeftHook();
void DisableSteerRightHook();

float GetDeltaTime();
static float steerAdd = 0.005f;
static float steerSmooth = 0.9f;
static float accSmooth = 0.1f;
void SteerSmooth();
void AccelerateResetHook();

void CurrentLapTimerAdvance();

void GameExitedHook();
void GameEnteredHook();

void SetVariable(const char* varName, int value);
int GetVariable(const char* varName);