#pragma once
#include "../plugin/PatternSolver.h"

struct ScriptParam {
	// TODO
	int type;
	unsigned int paramNameHash;
};

class ScriptParams {
public:
	short field0;
	short field2;
	ScriptParam* params;

	void PushString(unsigned int paramHash, char* data);
	void Delete();
};


ScriptParams* CreateScriptParams();


void RunScript(const char* name);
void RunScript(unsigned int nameHash, ScriptParams* params);
uintptr_t FindScriptGlobalObject(unsigned int nameHash);


extern float& m_fGameSpeed;