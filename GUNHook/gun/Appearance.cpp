#include "Appearance.h"
#include "../plugin/PatternSolver.h"

int LoadAppearance(int appearance)
{
	static uintptr_t pat = _pattern(PATID_LoadAppearance);
	if (pat)
		return ((int(__cdecl*)(int))pat)(appearance);

	return 0;
}
