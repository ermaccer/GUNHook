#include "eDirectInput8Hook.h"
#include <iostream>
#include "minhook/include/MinHook.h"
#include "plugin/GUNMenu.h"

IDirectInput8* eDirectInput8Hook::ms_pDirectInput;
LPDIRECTINPUTDEVICE8 eDirectInput8Hook::ms_pDevice;
bool eDirectInput8Hook::ms_bInit;
DIGetDeviceState eDirectInput8Hook::ms_pGetDeviceState;
DIGetDeviceData   eDirectInput8Hook::ms_pGetDeviceData;
uintptr_t	eDirectInput8Hook::ms_pFuncPtrs[2];
void eDirectInput8Hook::Init(HMODULE hMod)
{
	MH_Initialize();
	ms_bInit = false;
	if (!(DirectInput8Create(hMod, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&ms_pDirectInput, NULL) == DI_OK))
	{
		return;
	}

	if (!(ms_pDirectInput->CreateDevice(GUID_SysKeyboard, &ms_pDevice, NULL) == DI_OK))
	{
		ms_pDirectInput->Release();
		ms_pDirectInput = 0;
		return;
	}

	void** vTable = (void**)(*(int*)ms_pDevice);
	ms_pFuncPtrs[0] = (uintptr_t)vTable[9];
	ms_pFuncPtrs[1] = (uintptr_t)vTable[10];


	if (MH_CreateHook((void*)ms_pFuncPtrs[0], GetDeviceState_Hook, (LPVOID*)&ms_pGetDeviceState) != MH_OK || MH_EnableHook((void*)ms_pFuncPtrs[0]) != MH_OK)
	{
		return;
	}
	if (MH_CreateHook((void*)ms_pFuncPtrs[1], GetDeviceData_Hook, (LPVOID*)&ms_pGetDeviceData) != MH_OK || MH_EnableHook((void*)ms_pFuncPtrs[1]) != MH_OK)
	{
		return;
	}

	// no longer needed
	ms_pDirectInput->Release();
	ms_pDevice->Release();

	ms_pDirectInput = 0;
	ms_pDevice = 0;
}

void eDirectInput8Hook::Destroy()
{
	MH_DisableHook((void*)ms_pFuncPtrs[0]);
	MH_DisableHook((void*)ms_pFuncPtrs[1]);
}

HRESULT __stdcall eDirectInput8Hook::GetDeviceState_Hook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData)
{
	HRESULT orgResult = ms_pGetDeviceState(device, cbData, lpvData);
	if (TheMenu->m_bActive || TheMenu->ms_bEnableFreeCamera && cbData == 256)
		memset(lpvData, 0, cbData);

	return orgResult;
}

HRESULT __stdcall eDirectInput8Hook::GetDeviceData_Hook(IDirectInputDevice8* device, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	HRESULT orgResult = ms_pGetDeviceData(device, cbObjectData, rgdod, pdwInOut, dwFlags);
	return orgResult;
}
