#pragma once
#include "gun/Camera.h"
#include "plugin/Menu.h"
#include <Windows.h>


CCamera* Camera_Constructor_Hook();
BOOL WINAPI SetCursorPos_Hook(int X, int Y);

int LoadPlayerModel_Hook(const char* model);
int LoadPlayerAppearance_Hook(int appearance);