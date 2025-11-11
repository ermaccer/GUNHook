#include "Hooks.h"
#include "gun/Hash.h"
#include "plugin/PatternSolver.h"
#include "plugin/Menu.h"
#include "gun/Player.h"
#include "gun/Appearance.h"
#include "gun/Script.h"

CCamera* Camera_Constructor_Hook()
{
	static uintptr_t pat = _pattern(PATID_CameraConstructor);
	CCamera* cam = nullptr;
	if (pat)
	{
		cam = ((CCamera * (__cdecl*)())pat)();
		if (cam)
			TheCamera = cam;
	}

	return cam;
}

BOOL WINAPI SetCursorPos_Hook(int X, int Y)
{
	if (!TheMenu->m_bIsActive)
		return SetCursorPos(X, Y);
	return 0;
}


int LoadPlayerModel_Hook(const char* model)
{
	if (TheMenu->m_bChangePlayerSkin)
		model = TheMenu->m_selectedPlayerSkin.modelName;

	return LoadPlayerModel(model);
}

int LoadPlayerAppearance_Hook(int appearance)
{
	if (TheMenu->m_bChangePlayerSkin)
		appearance = FindScriptGlobalObject(_hash(TheMenu->m_selectedPlayerSkin.appearanceName));

	return LoadAppearance(appearance);
}
