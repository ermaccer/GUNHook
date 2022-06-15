#include "Camera.h"

CCamera* TheCamera;

Vector3D* CCamera::GetPosition()
{
	return (Vector3D*)(object + 104);
}

void CCamera::SetPosition(Vector3D pos)
{
	*(Vector3D*)(object + 104) = pos;
}

D3DMATRIX* CCamera::GetMatrix()
{
	return (D3DMATRIX*)(object + 120);
}
