#include "Player.h"
#include "../plugin/PatternSolver.h"

CPlayer* FindPlayer()
{
	static uintptr_t pat = _pattern(PATID_FindPlayer);
	if (pat)
		return *(CPlayer**)(*(unsigned int*)pat);

	return nullptr;
}

int LoadPlayerModel(const char* model)
{
	static uintptr_t pat = _pattern(PATID_LoadPlayerModel);
	if (pat)
		return ((int(__cdecl*)(const char*))pat)(model);

	return 0;
}

Vector3D* CPlayer::GetPosition()
{
	return (Vector3D*)((int)this + 104);
}

void CPlayer::SetPosition(Vector3D pos)
{
	*(Vector3D*)((int)this + 104) = pos;
}
