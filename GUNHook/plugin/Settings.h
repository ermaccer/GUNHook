#pragma once

struct eMouseSettings {
	int sens;
	bool invert_y;
	bool invert_x;
};

class eSettingsManager {
public:
	eSettingsManager();
	
	bool bEnableConsoleWindow;

	int iHookMenuOpenKey;
	int iHookSlowMoKey;
	int iToggleFreeCameraKey;
	int iToggleHUDKey;

	// free camera keys
	int iFreeCameraKeyForward;
	int iFreeCameraKeyBack;
	int iFreeCameraKeyLeft;
	int iFreeCameraKeyRight;
	int iFreeCameraKeyUp;
	int iFreeCameraKeyDown;

	int iFreeCameraKeyFOVPlus;
	int iFreeCameraKeyFOVMinus;

	int iFreeCameraKeyYawPlus;
	int iFreeCameraKeyYawMinus;

	int iFreeCameraKeyPitchPlus;
	int iFreeCameraKeyPitchMinus;

	int iFreeCameraKeyRollMinus;
	int iFreeCameraKeyRollPlus;

	eMouseSettings mouse;

	float fMenuScale;

	void SaveSettings();
	void ResetKeys();
};

extern eSettingsManager* SettingsMgr;