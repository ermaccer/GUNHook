#include "Script.h"
#include "Hash.h"
#include "../plugin/PatternHelper.h"

void RunScript(const char* name)
{
	static unsigned int script_pat = _pat("55 8B 6C 24 08 56 55 E8 ? ? ? ? 8B F0", 0, __FUNCTION__);
	((void(_cdecl*)(unsigned int, int, int, int, int, int))script_pat)(_hash(name), 0, 0, 0, 0, 0);
}
