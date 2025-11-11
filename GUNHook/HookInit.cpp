#include <windows.h>
#include <dinput.h>

#define HOOKED_FUNCTION DirectInput8Create
#define HOOKED_LIBRARY "dinput8.dll"

#include "utils/HookInit.hpp"