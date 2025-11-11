#include "..\utils\IniReader.h"
#include "Settings.h"
#include <Windows.h>

eSettingsManager* SettingsMgr = new eSettingsManager;

eSettingsManager::eSettingsManager()
{
	CIniReader ini("");
	CIniReader user("gunhook_user.ini");

	// user first
	bEnableConsoleWindow = ini.ReadBoolean("Settings", "bEnableConsoleWindow", true);

	iHookMenuOpenKey = user.ReadInteger("Settings", "iHookMenuOpenKey", -1);
	if (iHookMenuOpenKey == -1) iHookMenuOpenKey = ini.ReadInteger("Settings", "iHookMenuOpenKey", VK_F1);

	iHookSlowMoKey = user.ReadInteger("Settings", "iHookSlowMoKey", -1);
	if (iHookSlowMoKey == -1) iHookSlowMoKey = ini.ReadInteger("Settings", "iHookSlowMoKey", VK_F5);

	iToggleFreeCameraKey = user.ReadInteger("Settings", "iToggleFreeCameraKey", -1);
	if (iToggleFreeCameraKey == -1) iToggleFreeCameraKey = 0;

	iToggleHUDKey = user.ReadInteger("Settings", "iToggleHUDKey", -1);
	if (iToggleHUDKey == -1) iToggleHUDKey = 0;

	iFreeCameraKeyFOVPlus = user.ReadInteger("Settings", "iFreeCameraKeyFOVPlus", -1);
	if (iFreeCameraKeyFOVPlus == -1) iFreeCameraKeyFOVPlus = ini.ReadInteger("Settings", "iFreeCameraKeyFOVPlus", 0);

	iFreeCameraKeyFOVMinus = user.ReadInteger("Settings", "iFreeCameraKeyFOVMinus", -1);
	if (iFreeCameraKeyFOVMinus == -1) iFreeCameraKeyFOVMinus = ini.ReadInteger("Settings", "iFreeCameraKeyFOVMinus", 0);

	iFreeCameraKeyForward = user.ReadInteger("Settings", "iFreeCameraKeyForward", -1);
	if (iFreeCameraKeyForward == -1) iFreeCameraKeyForward = ini.ReadInteger("Settings", "iFreeCameraKeyForward", 0);

	iFreeCameraKeyBack = user.ReadInteger("Settings", "iFreeCameraKeyBack", -1);
	if (iFreeCameraKeyBack == -1) iFreeCameraKeyBack = ini.ReadInteger("Settings", "iFreeCameraKeyBack", 0);

	iFreeCameraKeyLeft = user.ReadInteger("Settings", "iFreeCameraKeyLeft ", -1);
	if (iFreeCameraKeyLeft == -1) iFreeCameraKeyLeft = ini.ReadInteger("Settings", "iFreeCameraKeyLeft", 0);

	iFreeCameraKeyRight = user.ReadInteger("Settings", "iFreeCameraKeyRight", -1);
	if (iFreeCameraKeyRight == -1) iFreeCameraKeyRight = ini.ReadInteger("Settings", "iFreeCameraKeyRight", 0);

	iFreeCameraKeyUp = user.ReadInteger("Settings", "iFreeCameraKeyUp", -1);
	if (iFreeCameraKeyUp == -1) iFreeCameraKeyUp = ini.ReadInteger("Settings", "iFreeCameraKeyUp", 0);

	iFreeCameraKeyDown = user.ReadInteger("Settings", "iFreeCameraKeyDown", -1);
	if (iFreeCameraKeyDown == -1) iFreeCameraKeyDown = ini.ReadInteger("Settings", "iFreeCameraKeyDown", 0);


	iFreeCameraKeyYawPlus = user.ReadInteger("Settings", "iFreeCameraKeyYawPlus", -1);
	if (iFreeCameraKeyYawPlus == -1) iFreeCameraKeyYawPlus = ini.ReadInteger("Settings", "iFreeCameraKeyYawPlus", 0);

	iFreeCameraKeyYawMinus = user.ReadInteger("Settings", "iFreeCameraKeyYawMinus", -1);
	if (iFreeCameraKeyYawMinus == -1) iFreeCameraKeyYawMinus = ini.ReadInteger("Settings", "iFreeCameraKeyYawMinus", 0);

	iFreeCameraKeyRollPlus = user.ReadInteger("Settings", "iFreeCameraKeyRollPlus", -1);
	if (iFreeCameraKeyRollPlus == -1) iFreeCameraKeyRollPlus = ini.ReadInteger("Settings", "iFreeCameraKeyRollPlus", 0);

	iFreeCameraKeyRollMinus = user.ReadInteger("Settings", "iFreeCameraKeyRollMinus", -1);
	if (iFreeCameraKeyRollMinus == -1) iFreeCameraKeyRollMinus = ini.ReadInteger("Settings", "iFreeCameraKeyRollMinus", 0);


	iFreeCameraKeyPitchPlus = user.ReadInteger("Settings", "iFreeCameraKeyPitchPlus", -1);
	if (iFreeCameraKeyPitchPlus == -1) iFreeCameraKeyPitchPlus = ini.ReadInteger("Settings", "iFreeCameraKeyPitchPlus", 0);

	iFreeCameraKeyPitchMinus = user.ReadInteger("Settings", "iFreeCameraKeyPitchMinus", -1);
	if (iFreeCameraKeyPitchMinus == -1) iFreeCameraKeyPitchMinus = ini.ReadInteger("Settings", "iFreeCameraKeyPitchMinus", 0);

	fMenuScale = user.ReadFloat("MenuSettings", "fMenuScale", 1.0f);

	if (fMenuScale < 1.0f)
		fMenuScale = 1.0f;

	mouse.sens = user.ReadInteger("Mouse", "Sensitivity", 10);
	mouse.invert_y = user.ReadBoolean("Mouse", "InvertY", false);
	mouse.invert_x = user.ReadBoolean("Mouse", "InvertX", false);
}

void eSettingsManager::SaveSettings()
{
	CIniReader user("gunhook_user.ini");
	user.WriteFloat("MenuSettings", "fMenuScale", fMenuScale);
	user.WriteInteger("Settings", "iHookMenuOpenKey", iHookMenuOpenKey);
	user.WriteInteger("Settings", "iHookSlowMoKey", iHookSlowMoKey);
	user.WriteInteger("Settings", "iFreeCameraKeyForward", iFreeCameraKeyForward);
	user.WriteInteger("Settings", "iFreeCameraKeyBack", iFreeCameraKeyBack);
	user.WriteInteger("Settings", "iFreeCameraKeyLeft", iFreeCameraKeyLeft);
	user.WriteInteger("Settings", "iFreeCameraKeyRight", iFreeCameraKeyRight);
	user.WriteInteger("Settings", "iFreeCameraKeyUp", iFreeCameraKeyUp);
	user.WriteInteger("Settings", "iFreeCameraKeyDown", iFreeCameraKeyDown);
	user.WriteInteger("Settings", "iFreeCameraKeyYawPlus", iFreeCameraKeyYawPlus);
	user.WriteInteger("Settings", "iFreeCameraKeyYawMinus", iFreeCameraKeyYawMinus);
	user.WriteInteger("Settings", "iFreeCameraKeyRollPlus", iFreeCameraKeyRollPlus);
	user.WriteInteger("Settings", "iFreeCameraKeyRollMinus", iFreeCameraKeyRollMinus);
	user.WriteInteger("Settings", "iFreeCameraKeyPitchPlus", iFreeCameraKeyPitchPlus);
	user.WriteInteger("Settings", "iFreeCameraKeyPitchMinus", iFreeCameraKeyPitchMinus);
	user.WriteInteger("Settings", "iFreeCameraKeyFOVPlus", iFreeCameraKeyFOVPlus);
	user.WriteInteger("Settings", "iFreeCameraKeyFOVMinus", iFreeCameraKeyFOVMinus);
	user.WriteInteger("Settings", "iToggleFreeCameraKey", iToggleFreeCameraKey);
	user.WriteInteger("Settings", "iToggleHUDKey", iToggleHUDKey);


	user.WriteInteger("Mouse", "Sensitivity", mouse.sens);
	user.WriteBoolean("Mouse", "InvertY", mouse.invert_y);
	user.WriteBoolean("Mouse", "InvertX", mouse.invert_x);

	CIniReader ini("gunhook.ini");
	ini.WriteBoolean("Settings", "bEnableConsoleWindow", bEnableConsoleWindow);
}

void eSettingsManager::ResetKeys()
{
	iHookMenuOpenKey = VK_F1;
	iToggleFreeCameraKey = 0;
	iToggleHUDKey = 0;
	iFreeCameraKeyForward = 104;
	iFreeCameraKeyBack = 98;
	iFreeCameraKeyLeft = 100;
	iFreeCameraKeyRight = 102;
	iFreeCameraKeyUp = 103;
	iFreeCameraKeyDown = 97;
	iFreeCameraKeyYawPlus = 39;
	iFreeCameraKeyYawMinus = 37;
	iFreeCameraKeyPitchPlus = 38;
	iFreeCameraKeyPitchMinus = 40;
	iFreeCameraKeyRollMinus = 99;
	iFreeCameraKeyRollPlus = 105;
	iHookSlowMoKey = VK_F5;
	iFreeCameraKeyFOVPlus = 107;
	iFreeCameraKeyFOVMinus = 109;
}
