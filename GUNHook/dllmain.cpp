// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"

#include "gui/dx9hook.h"
#include "gui/log.h"
#include "gui/notifications.h"

#include "utils/MemoryMgr.h"
#include "Hooks.h"
#include "plugin/PatternSolver.h"

#include "plugin/Menu.h"
#include "plugin/Settings.h"

#include "gun/Input.h"
#include "gun/Script.h"
#include "gun/Player.h"
#include "gun/Hash.h"
#include "gun/Appearance.h"
#include "gun/Matrix.h"

#include "helper/eMouse.h"

#include <iostream>
#include <Commctrl.h>

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "Comctl32.lib")


using namespace Memory::VP;

int GenericTrueReturn() { return 1; }
void _null() {}

void PluginProcess()
{
	while (true)
	{
		TheMenu->Process();
		eMouse::UpdateMouse();
		Notifications->Update();
		Sleep(1);
	}
}

void CopyCameraPosToMenu()
{
	if (!TheCamera)
		return;

	TheMenu->camPos = *TheCamera->GetPosition();
}

void CopyMenuPosToCamera()
{
	if (!TheCamera)
		return;

	*TheCamera->GetPosition() = TheMenu->camPos;
}


static int jmp_continue_camerapos = 0x0;
static int jmp_skip_camerapos = 0x0;
void __declspec(naked) UpdateCameraPosition_Hook()
{
	_asm pushad
	if (TheMenu->m_bFreeCam || TheMenu->m_bCustomCameraPos)
	{
		CopyMenuPosToCamera();
		_asm {
			popad
			jmp jmp_skip_camerapos
		}
	}
	else
	{
		CopyCameraPosToMenu();
		_asm {
			popad
			mov		[eax], ecx
			mov     edx, [esp + 72]
			jmp jmp_continue_camerapos
		}
	}

}



void Init()
{
	if (SettingsMgr->bEnableConsoleWindow)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}

	eLog::Message(__FUNCTION__, "INFO: GUNHook (%s | %s) Begin!", GUNHOOK_VERSION, __DATE__);
	Patch(_pattern(PATID_CameraConstructor_Hook), Camera_Constructor_Hook);
	Patch(*(int*)_pattern(PATID_SetCursorPos_Hook), SetCursorPos_Hook);

	jmp_continue_camerapos = _pattern(PATID_UpdateCamera_Hook) + 6;
	jmp_skip_camerapos = _pattern(PATID_UpdateCamera_Hook) + 23;
	InjectHook(_pattern(PATID_UpdateCamera_Hook), UpdateCameraPosition_Hook, HookType::Jump);

	InjectHook(_pattern(PATID_UpdateInput_Call1), &Input::Update_Hook);
	//InjectHook(_pattern(PATID_UpdateInput_Call2), &Input::Update_Hook);
	//InjectHook(_pattern(PATID_UpdateInput_Call3), &Input::Update_Hook);

	// patch dinput coop level
	Patch<char>(_pattern(PATID_DInput_Level) + 1, 4);
	Patch<char>(_pattern(PATID_DInput_Level2) + 1, 2);

	InjectHook(_pattern(PATID_LoadAppearance_Hook), LoadPlayerAppearance_Hook);
	InjectHook(_pattern(PATID_LoadPlayerModel_Hook), LoadPlayerModel_Hook);

	InjectHook(_pattern(PATID_Camera_UpdateMatrix_Hook), &Matrix::Copy_Hook);

	CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(PluginProcess), 0, NULL, 0);
	HANDLE h = 0;

	h = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(DX9Hook_Thread), 0, NULL, 0);

	if (!(h == nullptr)) CloseHandle(h);
}


bool ValidateGameVersion()
{
	if (PatternSolver::CheckMissingPatterns())
	{
		int nButtonPressed = 0;
		TASKDIALOGCONFIG config;
		ZeroMemory(&config, sizeof(TASKDIALOGCONFIG));

		const TASKDIALOG_BUTTON buttons[] = {
			{ IDOK, L"Launch anyway\nThe game might crash or have missing features!" },
			{ IDNO, L"Exit" }
		};
		config.cbSize = sizeof(config);

		config.dwFlags = TDF_ENABLE_HYPERLINKS | TDF_CAN_BE_MINIMIZED | TDF_USE_COMMAND_LINKS;
		config.pszMainIcon = TD_WARNING_ICON;

		config.pszWindowTitle = L"Warning";
		config.pszMainInstruction = L"GUNHook";
		config.pszContent = L"Could not start GUNHook!\n\n"
			L"One or more code patterns could not be found, this might indicate"
			L" that game version is not supported or the plugin has not been updated.\n\n"
			L"GUNHook officially is only tested with latest Steam and GOG versions.\n"
			L"Check log for more details.\n";


		config.pButtons = buttons;
		config.cButtons = ARRAYSIZE(buttons);

		if (SUCCEEDED(TaskDialogIndirect(&config, &nButtonPressed, NULL, NULL)))
		{
			switch (nButtonPressed)
			{
			case IDOK:
				return true;
				break;
			case IDNO:
				exit(0);
				break;
			default:
				break;
			}
		}
	}
	return true;
}

void OnInitializeHook()
{
	PatternSolver::Initialize();
	if (ValidateGameVersion())
	{
		Init();
	}
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
		MH_Initialize();
		eLog::Initialize();
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

