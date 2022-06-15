#pragma once

enum eMenuSubMenus {
	SUBMENU_SETTINGS,
	SUBMENU_SCRIPT,
	TOTAL_SUBMENUS
};

class GUNMenu {
public:
	bool	 m_bActive = false;
	bool	 m_bSubmenuActive[TOTAL_SUBMENUS] = {};
	bool     m_bPressingKey = false;
	bool	 m_bSlowMotion = false;
	bool	 m_bFOVMult = false;

	float	 m_fSlowMotionSpeed = 1.0f;
	float	 m_fFreeCamSpeed = 250.0f;
	float	 m_fFOVMult = 1.0f;


	int* m_pCurrentVarToChange = nullptr;

	void Init();
	void Draw();
	void Process();


	void DrawSettings();


	void DrawKeyBind(char* name, int* var);
	void KeyBind(int* var, char* bindName, char* name);


	static bool ms_bFreezeCamera;
	static bool ms_bEnableFreeCamera;

	static void ProcessFreeCamera();

	static void ResetGameSpeed();
};

extern GUNMenu* TheMenu;

float GetDeltaTime();