#include "menu.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

#include "..\imgui\imgui.h"
#include "..\utils.h"
#include "..\CVehicle.h"
#include "..\core\Vector4.h"

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

HookMenu* TheMenu = new HookMenu();
DDIAppClass* app = new DDIAppClass();

static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

}


HookMenu::HookMenu()
{
	//ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), ImGui::GetWindowFontSize(), ImVec2(100.f, 100.f), ImColor(255, 255, 0, 255), "Hello World", 0, 0.0f, 0);
	//ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(100, 100), ImColor(255, 255, 0, 255), "Hello, World!");
}

void HookMenu::Draw()
{
	if (!m_bIsActive)
		return;

	ImGui::GetIO().MouseDrawCursor = true;
	ImGui::Begin("Monster Trux Extreme: Offroad Edition Hook", &m_bIsActive, ImGuiWindowFlags_MenuBar);
	{
		ImGui::Text("Test Text");

		// TODO: Draw stuff here

	}
	ImGui::End();
}

void HookMenu::Update()
{


	UpdateKeys();
}

void HookMenu::UpdateKeys()
{
	if (m_bPressingKey)
		return;

	if (GetAsyncKeyState(HOOK_TOGGLEKEY) & 0x1) 
	{
		m_bIsActive = !m_bIsActive;
	}
}

void UpdateKeysGlobal()
{
	
	if (GetAsyncKeyState(VK_F4) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Failed to fix player's vehicle\n");
			return;
		}
		playerTruck->SetDamage(100);
	}
	if (GetAsyncKeyState(VK_F5) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Failed to fully-damage player's vehicle\n");
			return;
		}
		playerTruck->SetDamage(0);
	}
	if (GetAsyncKeyState(VK_F6) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Can't find player's vehicle!\n");
			return;
		}
		printf("-> %p\n", playerTruck);
	}


	if (GetAsyncKeyState(VK_F7) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Failed to set AI enabled to player's vehicle\n");
			return;
		}
		playerTruck->SetControllingFlags(1);
	}
	if (GetAsyncKeyState(VK_F8) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Failed to set AI disabled to player's vehicle\n");
			return;
		}
		playerTruck->SetControllingFlags(0);
	}
	// cheats keybinds
	if (GetAsyncKeyState(0x30) & 0x1)
	{
		ToggleVehicleCheat(0, NULL); // invincibility
	}
	if (GetAsyncKeyState(0x31) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		// trigger nitro
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Failed to trigger boost!\n");
			return;
		}
		playerTruck->TriggerNitro();
	}
	if (GetAsyncKeyState(0x32) & 0x1)
	{
		ToggleVehicleCheat(1, NULL); // inf time boost
	}
	/*
	if (GetAsyncKeyState(0x33) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		// trigger nitro
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Failed to set position to zero!\n");
			return;
		}
		playerTruck->SetPosition( Vector4(0, 0, 0, 0) );
	}
	*/
	if (GetAsyncKeyState(0x33) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		// trigger nitro
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Failed to get position!\n");
			return;
		}
		Vector4* pos = playerTruck->GetPosition();
		printf("%f, %f, %f\n", pos->X, pos->Y, pos->Z);
	}
	if (GetAsyncKeyState(0x34) & 0x1)
	{
		CVehicle* playerTruck = GetPlayerVehicle();
		// trigger nitro
		if (playerTruck == NULL | playerTruck == 0)
		{
			printf("Failed to give full nitro!\n");
			return;
		}
		playerTruck->SetNitroLevel(4);
	}
	/*
	if (GetAsyncKeyState(VK_F9) & 0x1)
	{
		if (SelectedVehicle == NULL | SelectedVehicle == 0)
		{
			printf("Failed to print vehicle type name\n");
			return;
		}
		printf("-> %s\n", GetVehicleNameFromType(SelectedVehicle->GetVehicleType()));
	}
	// selecting
	static const char* TRUCK_NOT_FOUND = "Not found!\n";
	if (GetAsyncKeyState(0x31) & 0x1)
	{
		SelectedVehicle = GetVehicleOfType(TRUCK_WRENCH);
		if (SelectedVehicle == NULL)
			printf(TRUCK_NOT_FOUND);
	}
	else if (GetAsyncKeyState(0x32) & 0x1) 
	{
		SelectedVehicle = GetVehicleOfType(TRUCK_DUMPER);
		if (SelectedVehicle == NULL)
			printf(TRUCK_NOT_FOUND);
	}
	else if (GetAsyncKeyState(0x33) & 0x1)
	{
		SelectedVehicle = GetVehicleOfType(TRUCK_DOOM);
		if (SelectedVehicle == NULL)
			printf(TRUCK_NOT_FOUND);
	}
	*/

}

void UpdateThread()
{
	while (true) {
		TheMenu->Update();
		UpdateKeysGlobal();
		Sleep(1);
	}
}