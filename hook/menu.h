#pragma once
#include "..\CVehicle.h"
#include <DirectXMath.h>
#include "..\DDIAppClass.h"
using namespace DirectX;

#define HOOK_VERSION "0.1"
#define HOOK_TOGGLEKEY VK_F1

class HookMenu {
public:
	bool m_bIsActive = false;
	bool m_bPressingKey = false;

	void OnActivate();
	void Draw();

	void Update();
	void UpdateKeys();



	HookMenu();
};

	
extern DDIAppClass* app;
extern HookMenu* TheMenu;
static CVehicle* SelectedVehicle = NULL;

void UpdateThread();
void UpdateKeysGlobal();
