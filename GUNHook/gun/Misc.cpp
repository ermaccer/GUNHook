#include "Misc.h"
#include "../plugin/PatternHelper.h"
#include <iostream>
void SetGameSpeed(float value)
{
	static unsigned int speed_pat = _pat("B8 04 00 00 00 03 D0 A1 ? ? ? ? 89 14 24", 8, __FUNCTION__);
	*(float*)(*(int*)speed_pat) = value;
}

void SetFOVMult(float value)
{
	static unsigned int fov_pat = _pat("DD D8 D9 C9 D9 C9 DE F9 D9 1D", 10, __FUNCTION__);
	*(float*)(*(int*)fov_pat) = value;
}
