#include "Matrix.h"
#include "..\plugin\Menu.h"

void Matrix::Copy(Matrix* a2)
{
	*this = *a2;
}

void Matrix::Copy_Hook(Matrix* a2)
{
	if (TheMenu->m_bFreeCam || TheMenu->m_bCustomCameraRot)
	{
		Copy(&TheMenu->m_mCameraMatrix);
	}
	else
		Copy(a2);
}
