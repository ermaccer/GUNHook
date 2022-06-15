#include <Windows.h>
#include "utils/MemoryMgr.h"
#include "Hooks.h"
#include "eDirectX9Hook.h"
#include "eDirectInput8Hook.h"
#include "plugin/PatternHelper.h"

using namespace Memory::VP;



void Init()
{
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);


    Patch(_pat("6A 00 68 ? ? ? ? 68 A5 A7 F5 FD", 3), Hooks::Camera_Constructor_Hook);
    Patch(*(int*)_pat("A1 ? ? ? ? 99 2B C2 D1 F8 50 FF 15", 13), Hooks::SetCursorPos_Hook);
    InjectHook(_pat("FF 15 ? ? ? ? 6A 20 E8 ? ? ? ? 3B C7", 8), eDirectX9Hook::Direct3DCreate9_Hook, PATCH_CALL);
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        Init();
        eDirectX9Hook::Init("CGun_TheGame");
        eDirectInput8Hook::Init(hMod);
        break;
    case DLL_PROCESS_DETACH:
        eDirectInput8Hook::Destroy();
        eDirectX9Hook::Destroy();
        break;
    }
    return TRUE;
}

