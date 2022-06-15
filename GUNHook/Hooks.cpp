#include "Hooks.h"
#include "gun/Hash.h"
#include "plugin/PatternHelper.h"

CCamera* Hooks::Camera_Constructor_Hook()
{
	static unsigned int cam_pat = _pat("56 68 E4 00 00 00 E8 ? ? ? ? 8B F0", 0, __FUNCTION__);

	CCamera* cam = ((CCamera*(__cdecl*)())cam_pat)();
	if (cam)
		TheCamera = cam;

	return cam;
}

BOOL __stdcall Hooks::SetCursorPos_Hook(int X, int Y)
{
	if (!TheMenu->m_bActive)
		return SetCursorPos(X, Y);
	return 0;
}
