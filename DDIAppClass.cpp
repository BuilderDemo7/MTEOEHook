#include "DDIAppClass.h"
#include "CVehicle.h"

#include <iostream>
#include <Windows.h>

// some functions pointers located at the .exe of Nestl� MTEOE:
// 001E6880 <- here
// and this address is on the dynamic memory of it:
// 005F00EE <- use CE's Memory Viewer or HxD (etc.)
void DDIAppClass::Kill(int exitCode)
{
	((void(__thiscall*)(int))0x5E6578)(exitCode);
}

void DDIAppClass::Unknown()
{
	((void(__thiscall*)(bool))0x565D84)(false);
}

float GetDeltaTime()
{
	return *(float*)(0x00661984);
}

void DoNotResetControlsForVehicle(CVehicle* vehicle)
{
	DontResetControlsForVehicle = vehicle;
}

void ToggleDontResetControls(bool state)
{
	DontResetControls = state;
}

void _declspec(naked) SteerSmooth()
{
	static int jump_continue = 0x0058A661;
	static int esiptr = 0;
	_asm
	{
		// original code
		//fstp dword ptr[esi + A4]

		fstp dword ptr[esi + 0xA0] // A0 = none (this is required to fix flickering trucks)
	    mov [esiptr],esi
	}
	// smooth it out by a value in the header of this file
	if (!DontResetControls && DontResetControlsForVehicle != (CVehicle*)esiptr) // checks if custom bool is not enabled
		*(float*)(esiptr + 0xA4) = *(float*)(esiptr + 0xA4) * steerSmooth;
	// continue
	_asm
	{
		jmp jump_continue
	}
}

void _declspec(naked) AccelerateResetHook()
{
	static int jump_continue = 0x0058A2BB;
	static int esiptr = 0;
	_asm
	{
		// original code
		//fstp dword ptr[esi + A8]

		fstp dword ptr[esi + 0xA0] // A0 = none (this is required to fix flickering trucks)
		mov[esiptr], esi
	}
	// smooth it out by a value in the header of this file
	if (!DontResetControls && (!((GetKeyState(InputConfig.AccelerateKey) & 0x800) && (GetKeyState(InputConfig.BrakeKey) & 0x800)) || InputConfig.CustomInput <= 0) && !DontResetThrottle && DontResetControlsForVehicle != (CVehicle*)esiptr) // checks if custom bool is not enabled
	{
	    *(float*)(esiptr + 0xA8) = *(float*)(esiptr + 0xA8) * accSmooth;
	}
	// continue
	_asm
	{
		jmp jump_continue
	}
}

void SetInputConfig(Input cfg)
{
	InputConfig = cfg;
}

// while (!paused) {}
void InputUpdate()
{
	CVehicle* player = GetPlayerVehicle();
	// custom input
	// throttle
	if (GetKeyState(InputConfig.AccelerateKey) & 0x800)
	{
		player->SetThrottle(-1);
	}
	else if (GetKeyState(InputConfig.BrakeKey) & 0x800)
	{
		player->SetThrottle(0.5f);
	}
	// steer
	if (GetKeyState(InputConfig.LeftKey) & 0x800)
	{
		playerSteer += steerAdd;
	}
	else if (GetKeyState(InputConfig.RightKey) & 0x800)
	{
		playerSteer -= steerAdd;
	}
	// otherwise, low it from the other code... or whatever.
	else
	{
		playerSteer = playerSteer / 1.01f; //player->GetSteer();
	}
	if (playerSteer > 1)
		playerSteer = 1;
	else if (playerSteer < -1)
		playerSteer = -1;

	player->SetSteer(playerSteer);
}

// called when the game is not paused a lot of times
void _declspec(naked) OnGameUpdateHook()
{
	static int jump_continue = 0x0059D076;
	static int _ptr = 0x00661984;
	_asm
	{
		// original code
		//fld dword ptr [00661984]

		fld dword ptr[_ptr]
	}
	InputUpdate();
	// continue
	_asm
	{
		jmp jump_continue
	}
}

// hook input fully
void _declspec(naked) DisableObeyAccelerateInputHook()
{
	static int jump_continue = 0x0058A292;
	_asm
	{
		fstp dword ptr[esi + 0xA0]
	}
	// continue
	_asm
	{
		jmp jump_continue
	}
}
void _declspec(naked) DisableObeyBrakeInputHook()
{
	static int jump_continue = 0x0058A31C;
	_asm
	{
		fstp dword ptr[esi + 0xA0]
	}
	// continue
	_asm
	{
		jmp jump_continue
	}
}

void _declspec(naked) SetSteerAxisHook()
{
	static int jump_continue = 0x0058A5E7;
	_asm
	{
		// original code
		//fst dword ptr [esi+000000A4]

		fst dword ptr[esi + 0xA0] // A0 = none (this is required to fix flickering trucks)
	}
	
	// continue
	_asm
	{
		jmp jump_continue
	}
}

void _declspec(naked) DisableSteerLeftHook()
{
	static int jump_continue = 0x0058A653;
	_asm
	{
		// original code
		//fstp dword ptr [esi+000000A4]

		fstp dword ptr[esi + 0xA0] // A0 = none (this is required to fix flickering trucks)
	}

	// continue
	_asm
	{
		jmp jump_continue
	}
}

void _declspec(naked) DisableSteerRightHook()
{
	static int jump_continue = 0x0058A622;
	_asm
	{
		// original code
		//fstp dword ptr [esi+000000A4]

		fstp dword ptr[esi + 0xA0] // A0 = none (this is required to fix flickering trucks)
	}

	// continue
	_asm
	{
		jmp jump_continue
	}
}