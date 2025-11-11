#include "Hash.h"
#include "../plugin/PatternSolver.h"

unsigned int _hash(const char* input)
{
	static uintptr_t pat = _pattern(PATID_Hash);
	if (pat)
	{
		return ((unsigned int(__cdecl*)(const char*))pat)(input);
	}
	return 0;
}
