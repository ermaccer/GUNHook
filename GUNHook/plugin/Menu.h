#pragma once
#include "../helper/eKeyboardMan.h"
#include <DirectXMath.h>
#include "../gun/Vector.h"
#include "../gun/Matrix.h"

using namespace DirectX;

enum eMenuSubMenus {
	SUBMENU_SETTINGS,
	SUBMENU_SCRIPT,
	TOTAL_SUBMENUS
};

struct PlayerSkin {
	char appearanceName[128] = {};
	char modelName[128] = {};
};


#define GUNHOOK_VERSION "0.2"

class GUNMenu {
public:
	bool	 m_bIsActive = false;
	bool	 m_bSubmenuActive[TOTAL_SUBMENUS] = {};
	bool     m_bPressingKey = false;
	bool	 m_bSlowMotion = false;
	bool	 m_bIsFocused = true;
	bool	 m_bCustomCameraPos = false;
	bool	 m_bCustomCameraRot = false;
	bool	 m_bFOVMult = false;
	bool	 m_bFreeCam = false;
	bool     m_bHideHUD = false;
	bool     m_bDisablePlayerInputInFreeCam = true;
	bool	 m_bChangePlayerSkin = false;
	bool	 m_bMouseControl = false;

	float	 m_fSlowMotionSpeed = 1.0f;
	float	 m_fFreeCameraSpeed = 250.0f;
	float	 m_fFreeCameraRotationSpeed = 0.15f;
	float	 m_fFOVMult = 1.0f;


	int* m_pCurrentVarToChange = nullptr;

	char playerSkin[256] = {};

	PlayerSkin m_selectedPlayerSkin = {};

	// camera

	Vector3D  camPos = {};
	Vector3D  camRot = {};
	Matrix	  m_mCameraMatrix = {};

	GUNMenu();

	void	 OnActivate();
	void	 OnToggleSlowMotion();
	void	 OnToggleFreeCamera();
	void	 OnToggleHUD();

	void	 Draw();
	void	 DrawPlayerTab();
	void	 DrawWorldTab();
	void	 DrawSpeedTab();
	void	 DrawCameraTab();
	void	 DrawScriptTab();
	void	 DrawMiscTab();

	void	 Process();


	void	 DrawSettings();


	void DrawKeyBind(char* name, int* var);
	void KeyBind(int* var, char* bindName, char* name);

	void ProcessFreeCamera();
	void ProcessRotationMatrix();
	static void ResetGameSpeed();
};

extern GUNMenu* TheMenu;
