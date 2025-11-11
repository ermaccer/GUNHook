#include "Misc.h"
#include "../plugin/PatternSolver.h"

void SetGameSpeed(float value)
{
	static uintptr_t pat = _pattern(PATID_SetGameSpeed);
	if (pat)
		*(float*)(*(int*)pat) = value;
}

void SetFOVMult(float value)
{
	static uintptr_t pat = _pattern(PATID_SetFOVMult);
	if (pat)
		*(float*)(*(int*)pat) = value;
}
