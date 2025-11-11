#include "Menu.h"
#include "Settings.h"

#include <map>
#include <string>

#include "../gui/notifications.h"
#include "../gui/imgui/imgui.h"
#include "../gui/gui_impl_dx9.h"

#include "../gun/Camera.h"
#include "../gun/Vector.h"
#include "../gun/Player.h"
#include "../utils/MemoryMgr.h"
#include "../gun/Hash.h"
#include "../gun/Script.h"
#include "../gun/Misc.h"
#include "../gun/Appearance.h"
#include "../helper/eKeyboardMan.h"
#include "../helper/eMouse.h"

using namespace Memory::VP;


std::map<std::string, std::string> mPlayerSkins = {
	{"Ped_Player_Default",  "cowboy_male"},
	{"Ped_Player_Final", "cowboy_Final"},
	{"Ped_Player_Resistance", "cowboy_Resistance"},
	{"Ped_Player_Survivor", "cowboy_Survivor"},
	{"Ped_Player_Start", "cowboy_Start" },
	{"Ped_Player_Blackfoot", "cowboy_indian" },
	{"Ped_Player_Deputy", "cowboy_Deputy" },
	{"Ped_Player_NewClothes", "cowboy_NewClothes" },
};


GUNMenu* TheMenu = new GUNMenu();

static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

}

GUNMenu::GUNMenu()
{
	sprintf(playerSkin, "Ped_Player_Default");
}

void GUNMenu::OnActivate()
{
	m_bIsActive ^= 1;
}

void GUNMenu::OnToggleSlowMotion()
{
	if (m_bIsActive)
		return;

	m_bSlowMotion ^= 1;

	if (!m_bSlowMotion)
		SetGameSpeed(1.0f);
}

void GUNMenu::OnToggleFreeCamera()
{
	if (m_bIsActive)
		return;

	m_bFreeCam ^= 1;
}

void GUNMenu::OnToggleHUD()
{
	m_bHideHUD ^= 1;

	if (m_bHideHUD)
		RunScript("hide_panel_stuff");
	else
		RunScript("show_panel_stuff");
}

void GUNMenu::Draw()
{
	if (!m_bIsActive)
		return;

	ImGui::GetIO().MouseDrawCursor = true;

	ImGui::Begin("GUNHook by ermaccer",&m_bIsActive, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			m_bSubmenuActive[SUBMENU_SETTINGS] = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::BeginMenu("About"))
			{
				ImGui::MenuItem("Version: " GUNHOOK_VERSION);
				ImGui::MenuItem("Date: " __DATE__);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

	}
	ImGui::EndMenuBar();

	if (ImGui::BeginTabBar("##tabs"))
	{
		if (ImGui::BeginTabItem("Player"))
		{
			DrawPlayerTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("World"))
		{
			DrawWorldTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Speed"))
		{
			DrawSpeedTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Camera"))
		{
			DrawCameraTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Script"))
		{
			DrawScriptTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Misc."))
		{
			DrawMiscTab();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

	if (m_bSubmenuActive[SUBMENU_SETTINGS])
		DrawSettings();
}

void GUNMenu::DrawPlayerTab()
{
	ImGui::Checkbox("Change Player Skin", &m_bChangePlayerSkin);
	ImGui::SameLine(); ShowHelpMarker("Set this option in the main menu. If the model doesn't change, start new game, then go back and load a save (model is cached after being loaded with no changes).");

	ImGui::PushItemWidth(-FLT_MIN);
	if (ImGui::BeginCombo("##skinlist", playerSkin))
	{
		for (auto m : mPlayerSkins)
		{
			bool is_selected = strcmp(playerSkin, m.first.c_str()) == 0;
			if (ImGui::Selectable(m.first.c_str(), is_selected))
			{
				sprintf(m_selectedPlayerSkin.appearanceName, m.first.c_str());
				sprintf(m_selectedPlayerSkin.modelName, m.second.c_str());
				sprintf(playerSkin, m.first.c_str());
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}

void GUNMenu::DrawWorldTab()
{
	if (ImGui::Button("Create Horse", { -FLT_MIN, 0 }))
		RunScript("RecreateHorse");
}

void GUNMenu::DrawSpeedTab()
{
	ImGui::Text("Gamespeed");
	ImGui::PushItemWidth(-FLT_MIN);
	ImGui::SliderFloat("##", &m_fSlowMotionSpeed, 0.001f, 10.0f);
	ImGui::PopItemWidth();


	if (ImGui::Checkbox("Enable", &m_bSlowMotion))
	{
		if (m_bSlowMotion) { SetGameSpeed(m_fSlowMotionSpeed); }
		else ResetGameSpeed();

	}

	ImGui::SameLine();
	ShowHelpMarker(eKeyboardMan::KeyToString(SettingsMgr->iHookSlowMoKey));

	if (m_fSlowMotionSpeed < 0.001f)
		m_fSlowMotionSpeed = 0.001f;

}

void GUNMenu::DrawCameraTab()
{
	if (!TheCamera)
		return;

	ImGui::Checkbox("Set Camera Position", &m_bCustomCameraPos);
	ImGui::InputFloat3("X | Y | Z", &TheCamera->GetPosition()->x);
	ImGui::Checkbox("Set Camera Rotation", &m_bCustomCameraRot);
	ImGui::InputFloat3("Pitch | Yaw | Roll", &camRot.x);

	if (ImGui::Checkbox("Set FOV Multiplier", &m_bFOVMult))
	{
		if (!m_bFOVMult)
			SetFOVMult(1.0f);
	}
	ImGui::InputFloat("FOV Multiplier", &m_fFOVMult);

	ImGui::Separator();
	if (ImGui::Checkbox("Enable Free Camera", &m_bFreeCam))
	{
		if (!m_bFreeCam)
		{
			m_fFOVMult = 1.0f;
			SetFOVMult(m_fFOVMult);
		}
	}
	ImGui::SameLine(); ShowHelpMarker("Allows to move camera with keyboard. Keys can be changed in the Settings menu or in the .ini file.");


	if (m_bFreeCam)
	{
		ImGui::Checkbox("Disable Player Input", &m_bDisablePlayerInputInFreeCam);
		ImGui::InputFloat("Free Camera Speed", &m_fFreeCameraSpeed);
		ImGui::InputFloat("Free Camera Rotation Speed", &m_fFreeCameraRotationSpeed);
		ImGui::Checkbox("Mouse Control", &m_bMouseControl);
	}
	ImGui::Separator();
	if (ImGui::Button("Teleport Player To Camera", { -FLT_MIN, 0 }))
	{
		*FindPlayer()->GetPosition() = *TheCamera->GetPosition();
	}
}

void GUNMenu::DrawScriptTab()
{
	static char txt[256];
	ImGui::Text("Function");
	ImGui::SameLine();
	ShowHelpMarker("Only functions without params/objects are supported!");
	ImGui::PushItemWidth(-FLT_MIN);
	ImGui::InputText("##", txt, sizeof(txt));
	ImGui::PopItemWidth();
	if (ImGui::Button("Execute", { -FLT_MIN, 0 }))
		RunScript(txt);
}

void GUNMenu::DrawMiscTab()
{
	if (ImGui::Button("Hide HUD"))
		RunScript("hide_panel_stuff");
	ImGui::SameLine();
	if (ImGui::Button("Show HUD"))
		RunScript("show_panel_stuff");
}

void GUNMenu::Process()
{
	ProcessFreeCamera();
	ProcessRotationMatrix();

	if (m_bSlowMotion)
		SetGameSpeed(TheMenu->m_fSlowMotionSpeed);

	if (m_bFOVMult || m_bFreeCam)
		SetFOVMult(m_fFOVMult);

	Notifications->Update();
}

void GUNMenu::DrawSettings()
{
	ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f }, ImGuiCond_Once, { 0.5f, 0.5f });
	ImGui::SetNextWindowSize({ 700,700 }, ImGuiCond_Once);
	ImGui::Begin("Settings", &m_bSubmenuActive[SUBMENU_SETTINGS]);

	static int settingID = 0;
	static const char* settingNames[] = {
		"Menu",
		"INI",
		"Keys",
		"Mouse"
	};

	enum eSettings {
		MENU,
		INI,
		KEYS,
		MOUSE
	};

	ImGui::BeginChild("##settings", { 12 * ImGui::GetFontSize(), 0 }, true);

	for (int n = 0; n < IM_ARRAYSIZE(settingNames); n++)
	{
		bool is_selected = (settingID == n);
		if (ImGui::Selectable(settingNames[n], is_selected))
			settingID = n;
		if (is_selected)
			ImGui::SetItemDefaultFocus();
	}

	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("##content", { 0, -ImGui::GetFrameHeightWithSpacing() });

	switch (settingID)
	{
	case MENU:
		ImGui::TextWrapped("All user settings are saved to gunhook_user.ini.");
		ImGui::Text("Menu Scale");
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::InputFloat("##", &SettingsMgr->fMenuScale);
		ImGui::PopItemWidth();
		break;
	case INI:
		ImGui::TextWrapped("These settings control GUNHook.ini options. Any changes require game restart to take effect.");
		ImGui::Checkbox("Debug Console", &SettingsMgr->bEnableConsoleWindow);
		break;
	case KEYS:
		if (m_bPressingKey)
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.3f, 1.f), "Press a key!");
		if (ImGui::Button("Reset Keys", { -FLT_MIN, 0 }))
		{
			SettingsMgr->ResetKeys();
			Notifications->SetNotificationTime(2500);
			Notifications->PushNotification("Keys reset! Remember to save.");
		}
		ImGui::Separator();
		ImGui::LabelText("##", "Core");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iHookMenuOpenKey, "Open/Close Menu", "menu");
		KeyBind(&SettingsMgr->iHookSlowMoKey, "Gamespeed Toggle", "slowmo");
		ImGui::Separator();
		ImGui::LabelText("##", "Camera");
		ImGui::Separator();	

		KeyBind(&SettingsMgr->iFreeCameraKeyFOVPlus, "FOV+", "fov_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyFOVMinus, "FOV-", "fov_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyYawPlus, "Yaw+", "ya_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYawMinus, "Yaw-", "ya_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchPlus, "Pitch+", "pi_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchMinus, "Pitch-", "pi_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollPlus, "Roll+", "r_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollMinus, "Roll-", "r_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyForward, "Forward", "x_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyBack, "Back", "x_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyLeft, "Left", "y_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRight, "Right", "y_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyUp, "Up", "z_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyDown, "Down", "z_minus");
		ImGui::Separator();
		ImGui::LabelText("##", "Misc");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iToggleFreeCameraKey, "Toggle Free Camera", "fcam");
		KeyBind(&SettingsMgr->iToggleHUDKey, "Toggle HUD", "thud");
		ImGui::Separator();
		
		if (m_bPressingKey)
		{
			eVKKeyCode result = eKeyboardMan::GetLastKey();

			if (result >= VK_BACKSPACE && result < VK_KEY_NONE)
			{
				*m_pCurrentVarToChange = result;
				m_bPressingKey = false;
			}

		}
		break;
	case MOUSE:
		ImGui::TextWrapped("All user settings are saved to gunhook_user.ini.");
		ImGui::Text("Sensitivity");
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::SliderInt("##", &SettingsMgr->mouse.sens, 1, 50);
		ImGui::PopItemWidth();
		ImGui::Checkbox("Invert X", &SettingsMgr->mouse.invert_x);
		ImGui::Checkbox("Invert Y", &SettingsMgr->mouse.invert_y);
		break;
	default:
		break;
	}

	if (ImGui::Button("Save", { -FLT_MIN, 0 }))
	{
		Notifications->SetNotificationTime(2500);
		Notifications->PushNotification("Settings saved!");
		GUIImplementationDX9::RequestFontReload();
		SettingsMgr->SaveSettings();
	}

	ImGui::EndChild();

	ImGui::End();
}

void GUNMenu::DrawKeyBind(char* name, int* var)
{
	ImGui::SameLine();

	static char butName[256] = {};
	sprintf(butName, "%s##key%s", eKeyboardMan::KeyToString(*var), name);
	if (ImGui::Button(butName))
	{
		m_bPressingKey = true;
		m_pCurrentVarToChange = var;
	}
}

void GUNMenu::KeyBind(int* var, char* bindName, char* name)
{
	ImGui::LabelText("##", bindName);
	DrawKeyBind(name, var);
}

void GUNMenu::ProcessFreeCamera()
{
	if (!m_bFreeCam)
		return;

	Matrix* matrix = TheCamera->GetMatrix();

	Vector3D fwd = matrix->forward;
	Vector3D strafe = matrix->right;
	Vector3D up = matrix->up;


	float delta = GUIImplementationDX9::GetDeltaTime();
	float speed = m_fFreeCameraSpeed;
	float rotSpeed = m_fFreeCameraRotationSpeed;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyForward))
		TheMenu->camPos += fwd * speed * delta * -1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyBack))
		TheMenu->camPos += fwd * speed * delta * 1;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyLeft))
		TheMenu->camPos += strafe * speed * delta * -1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRight))
		TheMenu->camPos += strafe * speed * delta * 1;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyUp))
		TheMenu->camPos += up * speed * delta * 1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyDown))
		TheMenu->camPos += up * speed * delta * -1;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawMinus))
		TheMenu->camRot.y += rotSpeed * delta;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawPlus))
		TheMenu->camRot.y -= rotSpeed * delta;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollMinus))
		TheMenu->camRot.z -= rotSpeed * delta;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollPlus))
		TheMenu->camRot.z += rotSpeed * delta;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchMinus))
		TheMenu->camRot.x -= rotSpeed * delta;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchPlus))
		TheMenu->camRot.x += rotSpeed * delta;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVPlus))
		TheMenu->m_fFOVMult += 0.1f;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVMinus))
		TheMenu->m_fFOVMult -= 0.1f;

	// mouse
	{
		if (!TheMenu->m_bIsActive && TheMenu->m_bMouseControl)
		{
			TheMenu->camRot.x += (eMouse::GetDeltaY() * 0.025f) * delta;
			TheMenu->camRot.y += -(eMouse::GetDeltaX() * 0.025f) * delta;
		}
	}
}

void GUNMenu::ProcessRotationMatrix()
{
	static XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	static XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(camRot.x, camRot.y, camRot.z);
	XMVECTOR camLookAt = XMVector3TransformCoord(defaultForward, rotMat);
	XMVECTOR camUp = XMVector3TransformCoord(defaultUp, rotMat);
	XMVECTOR camRight = XMVector3TransformCoord(defaultRight, rotMat);
	XMVector3Normalize(camLookAt);
	XMVector3Normalize(camUp);
	XMVector3Normalize(camRight);
	XMFLOAT3 cL, cU, cR;
	XMStoreFloat3(&cL, camLookAt);
	XMStoreFloat3(&cU, camUp);
	XMStoreFloat3(&cR, camRight);

	m_mCameraMatrix.forward = { cL.x, cL.y, cL.z };
	m_mCameraMatrix.up = { cU.x, cU.y, cU.z };
	m_mCameraMatrix.right = { cR.x, cR.y, cR.z };
}

void GUNMenu::ResetGameSpeed()
{
	SetGameSpeed(1.0f);
}