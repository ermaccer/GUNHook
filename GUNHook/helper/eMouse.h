#pragma once

class eMouse {
public:
	int x;
	int y;

	float deltaX;
	float deltaY;
	bool gotDelta;

	eMouse();

	void Clear();

	static void UpdateMouse();

	static eMouse mouse;


	static float  GetDeltaX();
	static float  GetDeltaY();

	static void LockMouse();
};

