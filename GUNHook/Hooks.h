#pragma once
#include "gun/Camera.h"
#include "plugin/GUNMenu.h"
#include <Windows.h>


namespace Hooks {
	CCamera* Camera_Constructor_Hook();
	BOOL __stdcall SetCursorPos_Hook(int X, int Y);
}