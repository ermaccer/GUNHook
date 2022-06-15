#include "GUNMenu.h"
#include "../gun/Camera.h"
#include "../Vector.h"
#include "../imgui/imgui.h"
#include "../gun/Player.h"
#include "../utils/MemoryMgr.h"
#include "../eDirectX9Hook.h"
#include "../gun/Hash.h"
#include "../gun/Script.h"
#include "../gun/Misc.h"
#include "../helper/eKeyboardMan.h"
#include "../SettingsMgr.h"
#include "../eNotifManager.h"

using namespace Memory::VP;

bool GUNMenu::ms_bFreezeCamera;
bool GUNMenu::ms_bEnableFreeCamera;
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

void GUNMenu::Init()
{
}

void GUNMenu::Draw()
{
	ImGui::Begin("GUNHook by ermaccer",&m_bActive, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Settings"))
			{
				m_bSubmenuActive[SUBMENU_SETTINGS] = true;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		if (ImGui::BeginTabBar("##tabs"))
		{
			if (ImGui::BeginTabItem("World"))
			{
				if (ImGui::Button("Create Horse"))
					RunScript("RecreateHorse");

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Speed"))
			{
				ImGui::Checkbox("Change Speed", &m_bSlowMotion);

				if (!m_bSlowMotion)
					ResetGameSpeed();

				ImGui::SameLine();
				{
					static char tmp[128] = {};
					sprintf(tmp, "Hotkey - %s", eKeyboardMan::KeyToString(SettingsMgr->iHookSlowMoKey));
					ShowHelpMarker(tmp);
				}
				
				ImGui::Separator();
				ImGui::Text("Value");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::SliderFloat("", &m_fSlowMotionSpeed, 0.001f, 10.0f);
				ImGui::PopItemWidth();

				if (m_fSlowMotionSpeed < 0.001f)
					m_fSlowMotionSpeed = 0.001f;

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Camera"))
			{
				Vector3D camPos;
				if (TheCamera)
				{
					ImGui::Checkbox("Free Camera", &ms_bEnableFreeCamera);
					ImGui::SameLine(); ShowHelpMarker("Allows to control camera with keyboard. Check settings for keys. Toggle \"Change FOV\" to be able to use FOV keys.");
					ImGui::Checkbox("Freeze Camera", &ms_bFreezeCamera);
					ImGui::SameLine(); ShowHelpMarker("Freezes position in place, allows player movement.");
					ImGui::Separator();
					ImGui::Text("Position");
					ImGui::InputFloat3("X | Y | Z", &TheCamera->GetPosition()->x);
					ImGui::Separator();
					ImGui::Checkbox("Change FOV", &m_bFOVMult);

					if (!m_bFOVMult)
						SetFOVMult(1.0f);

					if (m_bFOVMult)
					{
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::SliderFloat("", &m_fFOVMult, -20.000f, 20.0f);
						ImGui::PopItemWidth();
					}

					if (ms_bEnableFreeCamera)
					{
						ImGui::Separator();
						ImGui::Text("Free Camera Settings");
						ImGui::InputFloat("Speed", &m_fFreeCamSpeed);

						if (ImGui::Button("Teleport Player To Camera"))
						{
							*FindPlayer()->GetPosition() = *TheCamera->GetPosition();
						}
					}
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Script"))
			{
				
				static char txt[256];
				ImGui::Text("Function");
				ImGui::SameLine();
				ShowHelpMarker("Only functions without params/objects are supported!");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::InputText("", txt, sizeof(txt));
				ImGui::PopItemWidth();
				if (ImGui::Button("Execute"))
					RunScript(txt);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Misc."))
			{
				if (ImGui::Button("Hide HUD"))
					RunScript("hide_panel_stuff");
				if (ImGui::Button("Show HUD"))
					RunScript("show_panel_stuff");

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	if (m_bSubmenuActive[SUBMENU_SETTINGS])
		DrawSettings();
}

void GUNMenu::Process()
{
	if (ms_bFreezeCamera || ms_bEnableFreeCamera)
	{
		Nop(0x54746C, 2);
		Nop(0x547472, 3);
		Nop(0x547479, 3);
	}
	else
	{
		Patch<char>(0x54746C, 0x89);
		Patch<char>(0x54746C + 1, 0x08);

		Patch<char>(0x547472, 0x89);
		Patch<char>(0x547472 + 1, 0x50);
		Patch<char>(0x547472 + 2, 0x04);

		Patch<char>(0x547479, 0x89);
		Patch<char>(0x547479 + 1, 0x48);
		Patch<char>(0x547479 + 2, 0x08);
	}

	if (ms_bEnableFreeCamera)
		ProcessFreeCamera();

	if (TheMenu->m_bSlowMotion)
		SetGameSpeed(TheMenu->m_fSlowMotionSpeed);

	if (TheMenu->m_bFOVMult)
		SetFOVMult(TheMenu->m_fFOVMult);
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
		"Keys"
	};

	enum eSettings {
		MENU,
		KEYS,
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
		ImGui::InputFloat("", &SettingsMgr->fMenuScale);
		ImGui::PopItemWidth();
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
		ImGui::LabelText("", "Core");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iHookMenuOpenKey, "Open/Close Menu", "menu");
		KeyBind(&SettingsMgr->iHookSlowMoKey, "Gamespeed Toggle", "slowmo");
		ImGui::Separator();
		ImGui::LabelText("", "Camera");
		ImGui::Separator();	

		KeyBind(&SettingsMgr->iFreeCameraKeyFOVPlus, "FOV+", "fov_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyFOVMinus, "FOV-", "fov_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyForward, "Forward", "x_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyBack, "Back", "x_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyLeft, "Left", "y_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRight, "Right", "y_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyUp, "Up", "z_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyDown, "Down", "z_minus");
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
	default:
		break;
	}

	if (ImGui::Button("Save", { -FLT_MIN, 0 }))
	{
		Notifications->SetNotificationTime(2500);
		Notifications->PushNotification("Settings saved!");
		eDirectX9Hook::ms_bShouldReloadFonts = true;
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
	ImGui::LabelText("", bindName);
	DrawKeyBind(name, var);
}

void GUNMenu::ProcessFreeCamera()
{
	D3DMATRIX* matrix = TheCamera->GetMatrix();

	Vector3D fwd = { matrix->_41,matrix->_42,matrix->_43 };
	Vector3D strafe = { matrix->_21,matrix->_22,matrix->_23 };
	Vector3D up = { matrix->_31,matrix->_32,matrix->_33 };

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyForward))
		*TheCamera->GetPosition() += fwd * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * -1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyBack))
		*TheCamera->GetPosition() += fwd * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * 1;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyLeft))
		*TheCamera->GetPosition() += strafe * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * -1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRight))
		*TheCamera->GetPosition() += strafe * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * 1;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyUp))
		*TheCamera->GetPosition() += up * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * 1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyDown))
		*TheCamera->GetPosition() += up * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * -1;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVPlus))
		TheMenu->m_fFOVMult += 0.1f;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVMinus))
		TheMenu->m_fFOVMult -= 0.1f;
}

void GUNMenu::ResetGameSpeed()
{
	SetGameSpeed(1.0f);
}

float GetDeltaTime()
{
	// game is locked at 30
	return 1.0f / 30.0f;
}
