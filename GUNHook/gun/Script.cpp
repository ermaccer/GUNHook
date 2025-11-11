#include "Script.h"
#include "Hash.h"


ScriptParams* CreateScriptParams()
{
	static uintptr_t pat = _pattern(PATID_Script_CreateParams);

	ScriptParams* params = nullptr;

	if (pat)
		params = ((ScriptParams * (__cdecl*)(int, int))pat)(8, 1);

	if (params)
	{
		*(int*)(params + 4) = 0;
		params->field2 = 0;
	}
	return params;
}


void RunScript(const char* name)
{
	static uintptr_t pat = _pattern(PATID_RunScript);
	if (pat)
	{
		((void(__cdecl*)(unsigned int, int, int, int, int, int))pat)(_hash(name), 0, 0, 0, 0, 0);
	}
}

void RunScript(unsigned int nameHash, ScriptParams* params)
{
	static uintptr_t pat = _pattern(PATID_RunScript);
	if (pat)
	{
		((void(__cdecl*)(unsigned int, ScriptParams*, int, int, int, int))pat)(nameHash, params, 0, 0, 0, 0);
	}
}

uintptr_t FindScriptGlobalObject(unsigned int nameHash)
{
	static uintptr_t pat = _pattern(PATID_Script_FindGlobalObject);
	if (pat)
		return 	((uintptr_t(__cdecl*)(unsigned int))pat)(nameHash);

	return 0;
}

void ScriptParams::PushString(unsigned int paramHash, char* data)
{
	static uintptr_t pat = _pattern(PATID_Script_PushString);
	if (pat)
		((void(__thiscall*)(ScriptParams*, unsigned int, char*))pat)(this, paramHash, data);
}

void ScriptParams::Delete()
{
	static uintptr_t pat = _pattern(PATID_Script_DestroyParams1);
	if (pat)
		((void(__thiscall*)(ScriptParams*))pat)(this);

	static uintptr_t pat2 = _pattern(PATID_Script_DestroyParams2);
	if (pat2)
		((void(__cdecl*)(ScriptParams*))pat2)(this);

}
