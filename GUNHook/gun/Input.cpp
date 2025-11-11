#include "Input.h"
#include "../plugin/Menu.h"
#include "../plugin/PatternSolver.h"


void Input::Update(int a2)
{
	static uintptr_t pat = _pattern(PATID_UpdateInput);

	if (pat)
		((void(__thiscall*)(Input*, int))pat)(this, a2);
}

void Input::Update_Hook(int a2)
{
	if (TheMenu->m_bIsActive)
		return;

	if (TheMenu->m_bFreeCam && TheMenu->m_bDisablePlayerInputInFreeCam)
		return;

	Update(a2);
}
