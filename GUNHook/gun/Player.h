#pragma once
#include "Vector.h"

class CPlayer {
public:
	void* vTable;
	int objectName;
	char field_8;
	char field_9;
	char field_A;
	char field_B;
	int object;

	Vector3D* GetPosition();
	void	  SetPosition(Vector3D pos);
};

CPlayer* FindPlayer();

int LoadPlayerModel(const char* model);