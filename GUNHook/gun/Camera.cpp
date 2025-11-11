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

Matrix* CCamera::GetMatrix()
{
	return (Matrix*)(object + 136);
}
