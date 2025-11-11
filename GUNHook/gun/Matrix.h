#pragma once
#include "Vector.h"

class Matrix {
public:
	Vector3D right;
	float pad;
	Vector3D up;
	float pad2;
	Vector3D forward;
	float pad3;
	Vector3D pos;
	float pad4;


	void Copy(Matrix* a2);

	void Copy_Hook(Matrix* a2);
};