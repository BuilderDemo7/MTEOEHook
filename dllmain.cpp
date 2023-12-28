// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"
#include <iostream>

#include "utils.h"
#include "hook/menu.h"
#include "DDIAppClass.h"
#include "CVehicle.h"

// multiplayer
#include "client.h"

// ini
#include "IniReader.h"
#include "ini.h"

#include "d3hook\imguioverlay.hpp"

#define HOOKMETHOD_DUMMYDEVICE 0x01
#define HOOKMETHOD HOOKMETHOD_DUMMYDEVICE

#include "d3hook\hook.hpp"
#include "d3hook\injectionlock.hpp"
#if HOOKMETHOD == HOOKMETHOD_DUMMYDEVICE
#include "d3hook\dummydevice.hpp"
#endif

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")
#pragma warning(disable:4996)

void null() {}

// okay maybe I don't need this at all but I'm still disabling these
/*
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
*/

// D3D9 hooking stuff
// 0x5D0C1F20 is EndScene
LPCWSTR gWindowName = L"Monster Trux";
void RenderOverlay(IDirect3DDevice9* pDevice)
{
	imguioverlay::EndScene(pDevice, gWindowName);
	if (!imguioverlay::ReadyToRender()) return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// draws the menu
	TheMenu->Draw();

	// etc
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

typedef HRESULT(__stdcall *EndScene_t)(IDirect3DDevice9* pDevice);
EndScene_t originalEndScene = nullptr;
HRESULT __stdcall hookEndScene(IDirect3DDevice9* pDevice)
{
	printf("hookEndScene(IDirect3DDevice9* %p)",pDevice);
	RenderOverlay(pDevice);
	return originalEndScene(pDevice);
}

void HookD3D9()
{
	InjectHook(0x5D0C1F20, hookEndScene, PATCH_JUMP);
	Nop(0x5D0C1F20+0x5, 2);
	
	/*
	auto addrEndScene = FindEndScene();
	if (addrEndScene)
	{
		originalEndScene = HookWithTrampoline<EndScene_t>(hookEndScene, addrEndScene, 7);

		//InjectHook(0x5D0C1F20, hookEndScene, PATCH_JUMP);
		//Nop(0x5D0C1F20+0x5, 2);
	}
	else
	{
		printf("Failed to hook D3D9 EndScene!");
	}
	*/
}

bool StartMultiplayer(short port, const char* ip)
{
	bool status = ConnectTo(port,ip); //PrepareSocket(port, ip);
	return status;
}

typedef struct
{
	int Multiplayer = 0; // int bool
	const char* IP = "127.0.0.0";
	const char* Name = "Player";
	short Port = 56000;
	//int HostServer = 0; // int bool
} Online;

static int OnlineConfigHandler(void* user, const char* section, const char* name,
	const char* value)
{
	Online* pconfig = (Online*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("Online", "Multiplayer")) {
		pconfig->Multiplayer = atoi(value);
	}
	else if (MATCH("Online", "IP")) {
		pconfig->IP = strdup(value);
	}
	else if (MATCH("Online", "Name")) {
		pconfig->Name = strdup(value);
	}
	//else if (MATCH("Online", "HostServer")) {
	//	pconfig->HostServer = atoi(value);
	//}
	else if (MATCH("Online", "Port")) {
		pconfig->Port = (short)atoi(value);
	}
	else {
		//return 0;  /* unknown section/name, error */
	}
	return 1;
}

static int InputConfigHandler(void* user, const char* section, const char* name,
	const char* value)
{
	Input* pconfig = (Input*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("Input", "AccelerateKey")) {
		pconfig->AccelerateKey = atoi(value);
	}
	else if (MATCH("Input", "CustomInput")) {
		pconfig->CustomInput = atoi(value);
	}
	else if (MATCH("Input", "BrakeKey")) {
		pconfig->BrakeKey = atoi(value);
	}
	else if (MATCH("Input", "LeftKey")) {
		pconfig->LeftKey = atoi(value);
	}
	else if (MATCH("Input", "RightKey")) {
		pconfig->RightKey = atoi(value);
	}
	else {
		//return 0;  /* unknown section/name, error */
	}
	return 1;
}

void Hook()
{
	// Main
    AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	// Hook Main
	printf("Initializing...\n");

	CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(UpdateThread), nullptr, 0, nullptr);;

	printf("Injecting hooks...\n");
	InjectHook(0x0058F4DA, VehicleGet, PATCH_JUMP);
	Nop(0x0058F4DF, 4);
	// controls of this game are awful but I'm making it better with this!
	InjectHook(0x0058A65B, SteerSmooth, PATCH_JUMP);
	Nop(0x0058A660, 1);
	InjectHook(0x0058A2B5, AccelerateResetHook, PATCH_JUMP);
	Nop(0x0058A660, 1);

	Input iconfig;
	if (ini_parse("MTEOEHook.ini", InputConfigHandler, &iconfig) < 0) {
		printf("[INPUT] Can't load 'MTEOEHook.ini' configuration file\n");
		return;
	}
	SetInputConfig(iconfig);

	// hook input fully if
	if (iconfig.CustomInput >= 1) {
		printf("WARNING: Custom input enabled, the injections might only work better on a keyboard!!\n");
		InjectHook(0x0059D070, OnGameUpdateHook, PATCH_JUMP);
		Nop(0x0059D075, 1);
		/*
		InjectHook(0x0058A28C, DisableObeyAccelerateInputHook, PATCH_JUMP);
		Nop(0x0058A31B, 1);
		InjectHook(0x0058A316, DisableObeyBrakeInputHook, PATCH_JUMP);
		Nop(0x0058A291, 1);
		InjectHook(0x0058A5E1, SetSteerAxisHook, PATCH_JUMP);
		Nop(0x0058A5E6, 1);
		*/
		// steer input hook
		InjectHook(0x0058A64D, DisableSteerLeftHook, PATCH_JUMP);
		Nop(0x0058A652, 1);
		InjectHook(0x0058A61C, DisableSteerRightHook, PATCH_JUMP);
		Nop(0x0058A621, 1);
	}
	// ok this doesn't work so let me disable it
	//HookD3D9(); // *atempt to

	InjectHook(0x0040959A, GetPlayerSelectedVehIdHook, PATCH_JUMP);
	// (No NOPS required because the instructions are 5 bytes)

	printf("Successfully installed!\n\n");

	printf("List of test keybinds:\n\n");
	printf("    F4 - Attempt to fix player's vehicle in-game\n");
	printf("    F5 - Makes player's vehicle fully-damaged\n");
	printf("  == DEV ==\n");
	printf("    F6 - Prints the pointer to player's vehicle in-game\n");
	printf("  == AI TEST ==\n");
	printf("    F7 - Set AI enabled for player's vehicle in-game\n");
	printf("    F8 - Set AI disabled for player's vehicle in-game\n");
	printf("  == CHEATS ==\n");
	printf("    0 - Toggle player's vehicle is invincible to any damage\n");
	printf("    1 - Trigger boost (even if you don't have nitro)\n");
	printf("    2 - Trigger infinite time boost\n");
	printf("    3 - Print player's vehicle position\n");
	printf("    4 - Give player full nitro\n");

	// online multiplayer

	//CIniReader ini = CIniReader((char*)"");
	Online config;
	config.Multiplayer = 0;
	config.IP = "127.0.0.0";
	config.Port = 56000;
	// config.HostServer = 0;

	if (ini_parse("MTEOEHook.ini", OnlineConfigHandler, &config) < 0) {
		printf("[ONLINE] Can't load 'MTEOEHook.ini' configuration file\n");
		return;
	}

	if (config.Multiplayer>=1)
	{
		printf("Connecting to %s ...\n", config.IP);
		bool status = StartMultiplayer(config.Port, config.IP);
		if (!status)
		    printf("Connection failed.\n");
		else 
		{
			printf("Connection succeded!\n");
			int nameSize = sizeof(config.Name) * 8;
			if (nameSize > 255)
			{
				nameSize = 255;
				printf("WARNING: The client name's size can't be bigger than 255 characters!");
			}
			send(GetClientSocket(), config.Name, nameSize, 0);
		}
		//CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(WaitForConnection), nullptr, 0, nullptr);;
		//std::thread connectionThread = std::thread(WaitForConnection);
	}
}




extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		Hook();
	}
}
