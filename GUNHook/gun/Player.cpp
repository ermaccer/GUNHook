#include "Player.h"
#include "../plugin/PatternHelper.h"

CPlayer* FindPlayer()
{
	static unsigned int player_pat = _pat("83 EC 44 56 8B F1 8B 0D", 8, __FUNCTION__);
	return *(CPlayer**)(*(unsigned int*)player_pat);
}

Vector3D* CPlayer::GetPosition()
{
	return (Vector3D*)((int)this + 104);
}

void CPlayer::SetPosition(Vector3D pos)
{
	*(Vector3D*)((int)this + 104) = pos;
}
