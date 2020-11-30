
#pragma warning(disable : 4530)

#define Use_VMProtect 0
#define AllocCons 0

#include <Windows.h>
#include "sdk/VMProtectSDK.h"
#pragma comment(lib, "VMProtectSDK64.lib")
#include <psapi.h>
#include <d3d11.h>
#include <emmintrin.h>
#include "winternl.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <time.h>
#include <chrono>
#include <sstream>

int isSteam = 0;

#include "sdk/NewUtils.h"
#include "sdk/utils.h"
#include "sdk/safe_mem.h"
#include "sdk_new/math.h"
#include "Config/Config.h"
#include "sdk/game_sdk.h"
#include "NewGui.h"
#include "cheat_funcs/EspFuncs.h"

#include "cheat_funcs/MiscFuncs.h"
#include "cheat_funcs/EntityFuncs.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"


#include "Hook/MenuHook.h"



int ListBoxResultColor;
using namespace GUI;

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static ID3D11DeviceContext* g_pd3dDeviceImmediateContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;


static void ShowExampleMenuFile();

void HelpMarker(const char* desc)
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
ImVec4 tempColor;
void CreateRenderTarget()
{
	DXGI_SWAP_CHAIN_DESC sd;
	g_pSwapChain->GetDesc(&sd);
	ID3D11Texture2D* pBackBuffer;
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
	render_target_view_desc.Format = sd.BufferDesc.Format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &g_mainRenderTargetView);
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	pBackBuffer->Release();

}
DWORD WINAPI DetachDll(PVOID base)
{
	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 0);
}

#define AllocCons 0

typedef HRESULT(__fastcall* DX11PresentFn) (IDXGISwapChain* pSwapChain, UINT syncintreval, UINT flags);
DX11PresentFn oPresent = nullptr;

typedef HRESULT(__stdcall* DX11ResizeFn)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
DX11ResizeFn oResize = nullptr;

HRESULT __stdcall Resize(IDXGISwapChain* Swap, UINT a2, UINT a3, UINT a4, DXGI_FORMAT a5, UINT a6) {
	GUI::Render.ResetCanvas(); return oResize(Swap, a2, a3, a4, a5, a6);
}

HWND hWnd = NULL;
ID3D11Device* pD11Device = nullptr;
IDXGISwapChain* dxSwapChain = nullptr;
ID3D11DeviceContext* pD11DeviceContext = nullptr;
ID3D11RenderTargetView* pD11RenderTargetView = nullptr;

void ApplyImGuiIO(ImGuiIO& io)
{
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	static ImWchar ranges[] = { 0x0020, 0x00FF, 0x0100, 0x017f, 0 };
	char buffer[MAX_PATH];

		if (GetWindowsDirectoryA(buffer, MAX_PATH))
			io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Inconsolata-Bold.ttf", 15.0f);
}

void setStyle() {
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(251.f, 193.f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(251.f, 193.f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(251.f, 193.f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

bool InitD3DX11(IDXGISwapChain* dxSwapChain)
{
	if (!pD11Device || !pD11DeviceContext)
	{

		ImGui::CreateContext();
		ApplyImGuiIO(ImGui::GetIO());

		if (SUCCEEDED(dxSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pD11Device)))
		{
			dxSwapChain->GetDevice(__uuidof(pD11Device), (void**)&pD11Device);
			pD11Device->GetImmediateContext(&pD11DeviceContext);
		}

		DXGI_SWAP_CHAIN_DESC desc;
		dxSwapChain->GetDesc(&desc);
		hWnd = desc.OutputWindow;
		GUI::NextWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)GUI::WndProc));

		ImGui_ImplWin32_Init(desc.OutputWindow);
		ImGui_ImplDX11_Init(pD11Device, pD11DeviceContext);
		setStyle();


		return true;
	}

	return false;
}

class Misk
{
public:
	static Misk& Instance();
	void Functions();

private:
	Misk() {};
	~Misk() {};
};

class Menu
{
public:
	static Menu& Instance();
	void Functions();

private:
	Menu() {};
	~Menu() {};
};

#define MENU Menu::Instance()


Menu& Menu::Instance()
{
	static Menu instance{};
	return instance;
}

static int color = 1;
static int tabs = 1;
bool init = false;

bool menu_show = false;

void ChangeKeyAim(void* blank)
{
	Vars::AimBot::keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				Vars::AimBot::aimkey = i;
				Vars::AimBot::keystatus = 0;
				return;
			}
		}
	}
}

void ChangeKeyJump(void* blank)
{
	Vars::AimBot::jumpkeystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				Vars::AimBot::jumpkey = i;
				Vars::AimBot::jumpkeystatus = 0;
				return;
			}
		}
	}
}

ImGuiWindowFlags Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar;
ImGuiColorEditFlags ColorFlag = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaBar;

void Menu::Functions()
{
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowSize(ImVec2(Vars::Global::ScreenWidth, Vars::Global::ScreenHigh));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Overlay", static_cast<bool*>(nullptr), ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
	ImGui::Text("Welcome!", ImGui::GetIO().Framerate, ImVec4(255.f, 0.f, 0.f, 1.f));
	ImGui::Text("Cracked Build | 11/13/2020", ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::End();

	if (GUI::Visible && ImGui::Begin("        | Lawson#7753 |                                | Public Build |", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		ImVec2 siz = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetWindowPos();

		{
			ImGui::BeginChild("MENU", ImVec2(200, 375), true);

			if (ImGui::Button("Aimbot", ImVec2(170, 50)))
			{
				tabs = 1;


			}


			if (ImGui::Button("Weapon", ImVec2(170, 50)))
			{

				tabs = 2;

			}


			if (ImGui::Button("Player", ImVec2(170, 50)))
			{

				tabs = 3;

			}


			if (ImGui::Button("Rage", ImVec2(170, 50)))
			{

				tabs = 5;

			}

			if (ImGui::Button("Resource", ImVec2(170, 50)))
			{

				tabs = 4;

			}


			if (ImGui::Button("Misc", ImVec2(170, 50)))
			{

				tabs = 6;

			}



			ImGui::EndChild();
			ImGui::SameLine();

			if (tabs == 1)
			{
				ImGui::BeginChild("Aim", ImVec2(500, 375), true);

				ImGui::Checkbox("Enable Aimbot", &Vars::AimBot::Activate);
				if (Vars::AimBot::Activate) {
					ImGui::SameLine();
					std::stringstream stream;
					stream << std::hex << Vars::AimBot::aimkey;
					std::string aimkey = "Change Aim Key (" + stream.str() + ")";
					if (Vars::AimBot::keystatus == 1)
					{
						aimkey = "Press key to bind";
					}
					if (ImGui::Button(aimkey.c_str()))
					{
						if (Vars::AimBot::keystatus == 0)
						{
							CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ChangeKeyAim, nullptr, 0, nullptr);
						}
					}
				}
				ImGui::Checkbox("Body Aim", &Vars::AimBot::BoneToAim);
				ImGui::Checkbox("Visible Check", &Vars::AimBot::VisibleCheck);
				ImGui::Checkbox("Ignore Team", &Vars::AimBot::IgnoreTeam);
				ImGui::Checkbox("Ignore NPC", &Vars::AimBot::IgnoreNpc);
				//
				ImGui::Checkbox("Crosshair", &Vars::AimBot::Crosshair);
				ImGui::SameLine(190);
				ImGui::ColorEdit4("Crosshair Color", Vars::Color::CrosshairColors, ColorFlag);
				//
				ImGui::Checkbox("Field of View", &Vars::AimBot::DrawFov);
				ImGui::SameLine(190);
				ImGui::ColorEdit4("Color Fov", Vars::Color::Fov, ColorFlag);
				ImGui::Checkbox("Screen Field of View", &Vars::Misc::CustomFov);
				if (Vars::Misc::CustomFov) {
					ImGui::SliderFloat("Field of View Value", &Vars::Misc::GraphicsFov, 0.f, 180.f);
				}
				//		
				if (Vars::AimBot::DrawFov) {
					ImGui::SliderFloat("Set fov", &Vars::AimBot::Fov, 0.f, 400.f);
					ImGui::Checkbox("Blackout FOV", &Vars::AimBot::FillFov);
				}
				//
				ImGui::Checkbox("Distance", &Vars::AimBot::Distance);
				if (Vars::AimBot::Distance) {
					ImGui::SliderFloat("Set Distance", &Vars::AimBot::Range, 0.f, 400.f);
				}
				ImGui::EndChild();
				ImGui::EndMenu();
			}

			if (tabs == 2)
			{
				ImGui::BeginChild("Weapon", ImVec2(500, 375), true);
				ImGui::Checkbox("Set Automatic", &Vars::Weapon::SetAutomatic);
				ImGui::Checkbox("Shotgun Nospread", &Vars::Misc::ShotGunTochka);
				ImGui::Checkbox("No Recoil", &Vars::Weapon::NoRecoil);
				ImGui::Checkbox("RCS", &Vars::AimBot::RCS);
				if (Vars::AimBot::RCS)
				{
					ImGui::SliderFloat("Recoil", &Vars::Weapon::Recoil, 0.f, 100.f);
				}
				ImGui::Checkbox("No Spread", &Vars::Weapon::AntiSpread);
				ImGui::Checkbox("No Sway", &Vars::Weapon::NoSway);
				ImGui::Checkbox("Fast Bow", &Vars::Misc::SuperBow);
				ImGui::Checkbox("Instant Eoka", &Vars::Weapon::SuperEoka);
				ImGui::Checkbox("Long Melee", &Vars::Misc::LongHand);
				ImGui::EndMenu();
				ImGui::EndChild();
			}

			if (tabs == 3)
			{
				ImGui::BeginChild("Weapon", ImVec2(500, 375), true);
				ImGui::Checkbox("Ignore Bots", &Vars::PlayerEsp::IgnoreNpc);

				ImGui::Checkbox("Player Box", &Vars::PlayerEsp::ShowPlayerBox);
				ImGui::SameLine(190);
				ImGui::ColorEdit4("Box Colors", Vars::Color::BoxColors, ColorFlag);

				ImGui::Checkbox("Fill Box", &Vars::PlayerEsp::FillBox); ImGui::SameLine(190);
				ImGui::ColorEdit4("Filled Box Color", Vars::Color::BoxFilledColor, ColorFlag);

				ImGui::Checkbox("Player Name", &Vars::PlayerEsp::ShowPlayerName); ImGui::SameLine(190);
				ImGui::ColorEdit4("Player Name Color", Vars::Color::PlayerRenderStringColor, ColorFlag);

				ImGui::Checkbox("Player Bones", &Vars::PlayerEsp::Skeleton); ImGui::SameLine(190);
				ImGui::ColorEdit4("Bones", Vars::Color::SkeletonColors, ColorFlag);

				ImGui::Checkbox("Player Health", &Vars::PlayerEsp::ShowPlayerHealth); ImGui::SameLine(165);
				ImGui::ColorEdit4("Color Health Bar", Vars::Color::HpBar, ColorFlag);
				ImGui::SameLine(190);
				ImGui::ColorEdit4("Color Empty HP Bar", Vars::Color::EmptyHpBar, ColorFlag);

				ImGui::Checkbox("Player Weapon", &Vars::PlayerEsp::ShowPlayerWeapon); ImGui::SameLine(190);
				ImGui::ColorEdit4("Weapon Colors", Vars::Color::WeaponColors, ColorFlag);

				ImGui::Checkbox("Player Distance", &Vars::PlayerEsp::ShowPlayerDist);

				ImGui::Checkbox("Ignore Sleepers", &Vars::PlayerEsp::IgnoreSleepers); ImGui::SameLine(190);
				ImGui::ColorEdit4("Sleepers Color", Vars::Color::SleepersColor, ColorFlag);

				ImGui::Checkbox("Enable Snaplines", &Vars::PlayerEsp::SnapLines); ImGui::SameLine(190);
				ImGui::ColorEdit4("Tracers", Vars::Color::SnapColors, ColorFlag);

				ImGui::Checkbox("Corpse", &Vars::Visuals::Corpse); ImGui::SameLine(190);
				ImGui::ColorEdit4("Corpse Color", Vars::Color::CorpseColor, ColorFlag);

				ImGui::EndChild();
				ImGui::EndMenu();

			}

			if (tabs == 4)
			{
				ImGui::BeginChild("Weapon", ImVec2(500, 375), true);
				ImGui::Checkbox("Stone", &Vars::Visuals::Stone); ImGui::SameLine(190);
				ImGui::ColorEdit4("Stone Ore Color", Vars::Color::StoneColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Pickup Stone", &Vars::Visuals::PickUp_Stone); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Pickup Stone Color", Vars::Color::PickUp_StoneColor, ColorFlag);

				ImGui::Checkbox("Sulphur", &Vars::Visuals::Sulphur); ImGui::SameLine(190);
				ImGui::ColorEdit4("Sulphur Ore Color", Vars::Color::SulphurColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Pickup Sulfur", &Vars::Visuals::PickUp_Sulfur); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Pickup Sulfur Color", Vars::Color::PickUp_SulfurColor, ColorFlag);

				ImGui::Checkbox("Metal", &Vars::Visuals::Metal); ImGui::SameLine(190);
				ImGui::ColorEdit4("Metal Ore Color", Vars::Color::MetalColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Pickup Metal", &Vars::Visuals::PickUp_Metal); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Pickup Metal Color", Vars::Color::PickUp_MetalColor, ColorFlag);

				ImGui::Text("-------------------------------------------------------------------");

				ImGui::Checkbox("Mushrooms", &Vars::Visuals::Mushrooms); ImGui::SameLine(190);
				ImGui::ColorEdit4("Mushrooms Color", Vars::Color::MushroomsColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Hemp", &Vars::Visuals::Hemp); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Hemp Color", Vars::Color::HempColor, ColorFlag);


				ImGui::Checkbox("Pumpkin", &Vars::Visuals::Pumpkin); ImGui::SameLine(190);
				ImGui::ColorEdit4("Pumpkin Color", Vars::Color::PumpkinColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Stash", &Vars::Visuals::Stash);  ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Stash Color", Vars::Color::StashColor, ColorFlag);

				ImGui::Checkbox("Potato", &Vars::Visuals::Potato); ImGui::SameLine(190);
				ImGui::ColorEdit4("Potato Color", Vars::Color::PotatoColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Minicopter", &Vars::Visuals::Minicopter); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Minicopter Color", Vars::Color::MinicopterColor, ColorFlag);

				ImGui::Checkbox("Corn", &Vars::Visuals::Corn); ImGui::SameLine(190);
				ImGui::ColorEdit4("Corn Color", Vars::Color::CornColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Patrol Heli", &Vars::Visuals::PatrolHeli);  ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Patrol Helicopter Color", Vars::Color::PatrolHeliCol, ColorFlag);

				ImGui::Checkbox("Berry", &Vars::Visuals::Berry); ImGui::SameLine(190);
				ImGui::ColorEdit4("Berry Color", Vars::Color::BerryColor, ColorFlag);

				ImGui::Text("-------------------------------------------------------------------");

				ImGui::Checkbox("Wolf", &Vars::Animals::Wolf); ImGui::SameLine(190);
				ImGui::ColorEdit4("Wolf Color", Vars::Color::WolfColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Turret", &Vars::Visuals::Turret); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Turret Color", Vars::Color::TurretColor, ColorFlag);

				ImGui::Checkbox("Deer", &Vars::Animals::Deer); ImGui::SameLine(190);
				ImGui::ColorEdit4("Deer Color", Vars::Color::DeerColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Guntrap", &Vars::Visuals::Guntrap); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Guntrap Color", Vars::Color::GuntrapColor, ColorFlag);

				ImGui::Checkbox("Horse", &Vars::Animals::Horse); ImGui::SameLine(190);
				ImGui::ColorEdit4("Horse Color", Vars::Color::HorseColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Backpack", &Vars::Visuals::Backpack); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Backpack Color", Vars::Color::BackpackColor, ColorFlag);

				ImGui::Checkbox("Chicken", &Vars::Animals::Chicken);  ImGui::SameLine(190);
				ImGui::ColorEdit4("Chicken Color", Vars::Color::ChickenColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Airdrop", &Vars::Visuals::Airdrop); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Airdrop Color", Vars::Color::AirdropColor, ColorFlag);

				ImGui::Checkbox("Boar", &Vars::Animals::Boar); ImGui::SameLine(190);
				ImGui::ColorEdit4("Boar Color", Vars::Color::BoarColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Barrel", &Vars::Visuals::Barrel);  ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Barrel Color", Vars::Color::BarrelCol, ColorFlag);

				ImGui::Checkbox("Bears", &Vars::Animals::Bears); ImGui::SameLine(190);
				ImGui::ColorEdit4("Bears Color", Vars::Color::BearsColor, ColorFlag);

				ImGui::Text("-------------------------------------------------------------------");
				 
				ImGui::Checkbox("Cupboard ", &Vars::Visuals::Cupboard); ImGui::SameLine(190);
				ImGui::ColorEdit4("Cupboard Color", Vars::Color::CupboardColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Cars", &Vars::technique::cars); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Cars Color", Vars::Color::CarsCol, ColorFlag);

				ImGui::Checkbox("Large Wood Box ", &Vars::Visuals::LWB); ImGui::SameLine(190);
				ImGui::ColorEdit4("Large Wood Box Color", Vars::Color::LWBColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Rowboat", &Vars::technique::Rowboat); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Rowboat Color", Vars::Color::RowboatCol, ColorFlag);

				ImGui::Checkbox("Bag Loot", &Vars::Visuals::bagloot); ImGui::SameLine(190);
				ImGui::ColorEdit4("Bag Loot Color", Vars::Color::BagLootColor, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("RHIB", &Vars::technique::RHIB); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("RHIB Color", Vars::Color::RHIBCol, ColorFlag);

				ImGui::Checkbox("Camp Fire", &Vars::Visuals::campfire); ImGui::SameLine(190);
				ImGui::ColorEdit4("Camp Fire Color", Vars::Color::CampFireCol, ColorFlag);

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Kayak", &Vars::technique::Kayak); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Kayak Color", Vars::Color::KayakCol, ColorFlag);

				ImGui::Checkbox("Furnace", &Vars::Visuals::Furnace);  ImGui::SameLine(190);
				ImGui::ColorEdit4("Furnace Color", Vars::Color::FurnaceCol, ColorFlag); 

				ImGui::SameLine(100, 180);
				ImGui::Checkbox("Cargo", &Vars::Visuals::Cargo); ImGui::SameLine(100, 330);
				ImGui::ColorEdit4("Cargo Color", Vars::Color::CargoCol, ColorFlag);

				ImGui::Text("-------------------------------------------------------------------");

				ImGui::EndChild();
				ImGui::EndMenu();
			}

			if (tabs == 5)
			{
				{
					ImGui::BeginChild("Weapon", ImVec2(500, 375), true);
					ImGui::Checkbox("Fat Bullet", &Vars::Misc::FatBullet);
			    //	ImGui::Checkbox("Magic Bullet (HEAD)", &Vars::AimBot::mBullet);
				//	ImGui::Checkbox("pSilent", &Vars::AimBot::pSilent);
					ImGui::Checkbox("Silent Aim", &Vars::AimBot::SilentAim);
					if (Vars::AimBot::SilentAim) {

						ImGui::Checkbox("Silent Body", &Vars::AimBot::SilentBody);
					}
					ImGui::Checkbox("Silent FOV", &Vars::AimBot::DrawSilentFOV);
					if (Vars::AimBot::DrawSilentFOV) {
						ImGui::SliderFloat("Recommended (600)", &Vars::AimBot::SilentFov, 0.f, 600.f);
					}
					ImGui::Checkbox("Through Terrain Walls", &Vars::AimBot::ThroughWall);
				}
				ImGui::EndChild();
				ImGui::EndMenu();
			}

			if (tabs == 6)
			{
				{
					ImGui::BeginChild("Weapon", ImVec2(500, 375), true);
					ImGui::Checkbox("Fake Admin", &Vars::Misc::FakeAdmin);
					ImGui::SameLine(100, 180);
					ImGui::Checkbox("WaterBoost", &Vars::Misc::WaterBoost);

					ImGui::Checkbox("Aim in Jump", &Vars::AimBot::JumpShot);
					ImGui::SameLine(100, 180);
					ImGui::Checkbox("Always Day", &Vars::Misc::AlwaysDay);

					
					ImGui::Checkbox("Enable Spiderman", &Vars::Misc::SpiderMan);
					ImGui::SameLine(100, 180);
					ImGui::Checkbox("HighJump (Middle Mouse)", &Vars::Misc::HighJump);
					if (Vars::Misc::HighJump)
					{
						ImGui::SliderFloat("Set HighJump", &Vars::Misc::JumpValue, 3.f, 0.f);
					}

					ImGui::Checkbox("WalkOnWater (Bind: X)", &Vars::Misc::WalkOnWater);
					ImGui::SameLine(100, 180);
					ImGui::Checkbox("No Heavy Reduct", &Vars::Misc::NoHeavyReduct);

					ImGui::Checkbox("No Block Aiming", &Vars::Misc::NoBlockAiming);
					ImGui::SameLine(100, 180);
					ImGui::Checkbox("Custom Time", &Vars::Misc::CustomTime);
					if (Vars::Misc::CustomTime) {
						ImGui::SliderFloat("Time", &Vars::Misc::Time, 0.f, 12.f);
					}
					
					ImGui::Checkbox("Speed up Movement", &Vars::Misc::SpeedHack);
					if (Vars::Misc::SpeedHack)
					{
						ImGui::SliderFloat("Set Speed Value", &Vars::Misc::SpeedValue, 0.f, 100.f);
					}
					ImGui::SameLine(100, 180);
					ImGui::Checkbox("Enable Thirdperson", &Vars::Misc::IsThirdPerson);

					ImGui::Checkbox("FakeLag", &Vars::Misc::FakeLag);
					ImGui::SameLine(100, 180);
					ImGui::Checkbox("AntiAim", &Vars::Misc::AntiAim);

					ImGui::Checkbox("Instant Compound", &Vars::Misc::compound);
					ImGui::SameLine(100, 180);
					ImGui::Checkbox("SilentWalk", &Vars::Misc::SilentWalk);
					ImGui::EndMenu();
					ImGui::EndChild();
				}

			}
		}
	}
}

const char* CheatColorManager[] =
{
	"CrosshairColors", "Fov", "LWBColor", "BoxColors", "SnapColors",
	"SleepersColor", "WoundedColor", "PlayerRenderStringColor","HpBar", "EmptyHpBar"
	"SkeletonColors", "WeaponColors", "BotBoxColors", "CorpseColor", "StoneColor",
	"SulphurColor", "MetalColor", "PickUp_StoneColor", "PickUp_SulfurColor", "PickUp_MetalColor",
	"HempColor", "MushroomsColor", "PickUp_WoodColor", "StashColor", "MinicopterColor",
	"TurretColor", "GuntrapColor", "BackpackColor", "AirdropColor", "BagLootColor",
	"BoxFilledColor", "CupboardColor", "LWBColor","CampFireCol", "BarrelCol",
	"CargoCol", "PatrolHeliCol", "WolfColor", "DeerColor", "HorseColor",
	"ChickenColor", "BoarColor", "BearsColor", "Cars", "RowboatCol",
	"KayakCol", "PumpkinColor", "PotatoColor", "CornColor", "BerryColor",
};

void CheatManager()
{

	ImGui::BeginChild(932, ImVec2(515.f, 230.f), true, Flags);
	{

		ImGui::Text("Color Manager");
		ImGui::Separator();
		ImGui::Spacing();
		ImGuiWindowFlags ColorPicker = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_DisplayHSV;
		ImGui::SameLine(40.f, -2);
		switch (ListBoxResultColor)
		{
		case 0:
			ImGui::ColorPicker4(("1"), Vars::Color::CrosshairColors, ColorPicker);
			break;
		case 1:
			ImGui::ColorPicker4(("2"), Vars::Color::Fov, ColorPicker);
			break;
		case 2:
			ImGui::ColorPicker4(("3"), Vars::Color::LWBColor, ColorPicker);
			break;
		case 3:
			ImGui::ColorPicker4(("4"), Vars::Color::BoxColors, ColorPicker);
			break;
		case 4:
			ImGui::ColorPicker4(("5"), Vars::Color::SnapColors, ColorPicker);
			break;
		case 5:
			ImGui::ColorPicker4(("6"), Vars::Color::SleepersColor, ColorPicker);
			break;
		case 6:
			ImGui::ColorPicker4(("7"), Vars::Color::WoundedColor, ColorPicker);
			break;
		case 7:
			ImGui::ColorPicker4(("8"), Vars::Color::PlayerRenderStringColor, ColorPicker);
			break;
		case 8:
			ImGui::ColorPicker4(("9"), Vars::Color::HpBar, ColorPicker);
			break;
		case 9:
			ImGui::ColorPicker4(("10"), Vars::Color::SkeletonColors, ColorPicker);
			break;
		case 10:
			ImGui::ColorPicker4(("11"), Vars::Color::EmptyHpBar, ColorPicker);
			break;
		case 11:
			ImGui::ColorPicker4(("12"), Vars::Color::WeaponColors, ColorPicker);
			break;
		case 12:
			ImGui::ColorPicker4(("13"), Vars::Color::StoneColor, ColorPicker);
			break;
		case 13:
			ImGui::ColorPicker4(("14"), Vars::Color::CorpseColor, ColorPicker);
			break;
		case 14:
			ImGui::ColorPicker4(("15"), Vars::Color::BotBoxColors, ColorPicker);
			break;
		case 15:
			ImGui::ColorPicker4(("16"), Vars::Color::SulphurColor, ColorPicker);
			break;
		case 16:
			ImGui::ColorPicker4(("17"), Vars::Color::MetalColor, ColorPicker);
			break;
		case 17:
			ImGui::ColorPicker4(("18"), Vars::Color::PickUp_StoneColor, ColorPicker);
			break;
		case 18:
			ImGui::ColorPicker4(("19"), Vars::Color::PickUp_SulfurColor, ColorPicker);
			break;
		case 19:
			ImGui::ColorPicker4(("20"), Vars::Color::PickUp_MetalColor, ColorPicker);
			break;
		case 20:
			ImGui::ColorPicker4(("21"), Vars::Color::HempColor, ColorPicker);
			break;
		case 21:
			ImGui::ColorPicker4(("22"), Vars::Color::MushroomsColor, ColorPicker);
			break;
		case 22:
			ImGui::ColorPicker4(("23"), Vars::Color::PickUp_WoodColor, ColorPicker);
			break;
		case 23:
			ImGui::ColorPicker4(("24"), Vars::Color::StashColor, ColorPicker);
			break;
		case 24:
			ImGui::ColorPicker4(("25"), Vars::Color::MinicopterColor, ColorPicker);
			break;
		case 25:
			ImGui::ColorPicker4(("26"), Vars::Color::TurretColor, ColorPicker);
			break;
		case 26:
			ImGui::ColorPicker4(("27"), Vars::Color::GuntrapColor, ColorPicker);
			break;
		case 27:
			ImGui::ColorPicker4(("28"), Vars::Color::BackpackColor, ColorPicker);
			break;
		case 28:
			ImGui::ColorPicker4(("29"), Vars::Color::AirdropColor, ColorPicker);
			break;
		case 29:
			ImGui::ColorPicker4(("30"), Vars::Color::BagLootColor, ColorPicker);
			break;
		case 30:
			ImGui::ColorPicker4(("31"), Vars::Color::BoxFilledColor, ColorPicker);
			break;
		case 31:
			ImGui::ColorPicker4(("32"), Vars::Color::CupboardColor, ColorPicker);
			break;
		case 32:
			ImGui::ColorPicker4(("33"), Vars::Color::CampFireCol, ColorPicker);
			break;
		case 33:
			ImGui::ColorPicker4(("34"), Vars::Color::BarrelCol, ColorPicker);
			break;
		case 34:
			ImGui::ColorPicker4(("35"), Vars::Color::CargoCol, ColorPicker);
			break;
		case 35:
			ImGui::ColorPicker4(("36"), Vars::Color::FurnaceCol, ColorPicker);
			break;
		case 36:
			ImGui::ColorPicker4(("37"), Vars::Color::PatrolHeliCol, ColorPicker);
			break;
		case 37:
			ImGui::ColorPicker4(("38"), Vars::Color::WolfColor, ColorPicker);
			break;
		case 38:
			ImGui::ColorPicker4(("39"), Vars::Color::DeerColor, ColorPicker);
			break;
		case 39:
			ImGui::ColorPicker4(("40"), Vars::Color::HorseColor, ColorPicker);
			break;
		case 40:
			ImGui::ColorPicker4(("41"), Vars::Color::ChickenColor, ColorPicker);
			break;
		case 41:
			ImGui::ColorPicker4(("42"), Vars::Color::BoarColor, ColorPicker);
			break;
		case 42:
			ImGui::ColorPicker4(("43"), Vars::Color::BearsColor, ColorPicker);
			break;
		case 43:
			ImGui::ColorPicker4(("44"), Vars::Color::CarsCol, ColorPicker);
			break;
		case 44:
			ImGui::ColorPicker4(("45"), Vars::Color::RowboatCol, ColorPicker);
			break;
		case 45:
			ImGui::ColorPicker4(("46"), Vars::Color::KayakCol, ColorPicker);
			break;
		case 46:
			ImGui::ColorPicker4(("47"), Vars::Color::PumpkinColor, ColorPicker);
			break;
		case 47:
			ImGui::ColorPicker4(("48"), Vars::Color::PotatoColor, ColorPicker);
			break;
		case 48:
			ImGui::ColorPicker4(("49"), Vars::Color::CornColor, ColorPicker);
			break;
		case 49:
			ImGui::ColorPicker4(("50"), Vars::Color::BerryColor, ColorPicker);
			break;
		}
		ImGui::SameLine(280.f, -2);
		ImGui::ListBox((""), &ListBoxResultColor, CheatColorManager, IM_ARRAYSIZE(CheatColorManager));
	}
	ImGui::EndChild();
}

#include <Winuser.h>
#include <tchar.h>
#include <iostream>
using namespace std;

HMODULE g_hModule;
PDWORD64 origPresent = 0;
PDWORD64 origResize = 0;
void hookD3D11PresentOld(IDXGISwapChain* pSwapChain);


HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (Vars::Other::bUnloading)
	{
		GUI::NextWndProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)GUI::NextWndProc);
		*(uintptr_t*)origPresent = (uintptr_t)oPresent;
		*(uintptr_t*)origResize = (uintptr_t)oResize;

		auto result = oPresent(pSwapChain, SyncInterval, Flags);
		HANDLE handle = CreateThread(nullptr, NULL, DetachDll, g_hModule, NULL, nullptr);
		CloseHandle(handle);
		return result;
	}
	else
	{
		if (GetAsyncKeyState(VK_HOME))
			Vars::Other::bUnloading = true;
	}

	dxSwapChain = pSwapChain;

	if (!InitD3DX11(dxSwapChain))
	{
		ID3D11Texture2D* renderTargetTexture = nullptr;
		if (!pD11RenderTargetView)
		{
			if (SUCCEEDED(dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
			{
				pD11Device->CreateRenderTargetView(renderTargetTexture, nullptr, &pD11RenderTargetView);
				renderTargetTexture->Release();
			}
		}
	}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		MENU.Functions();

		hookD3D11PresentOld(pSwapChain);

		if (GUI::Render.NewFrame(pSwapChain))
		{
			if (Vars::AimBot::DrawFov)
				DrawFOV();

			if (Vars::AimBot::Crosshair)
				Crosshair();

			GUI::Render.EndFrame();
		}

		pD11DeviceContext->OMSetRenderTargets(1, &pD11RenderTargetView, nullptr);
		ImGui::Render();

		if (pD11RenderTargetView)
		{
			pD11RenderTargetView->Release();
			pD11RenderTargetView = nullptr;
		}

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return oPresent(pSwapChain, SyncInterval, Flags);

	
}
HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

	if (SUCCEEDED(ret))
		(*ppDevice)->GetImmediateContext(ppContext);

	return ret;
}

void hookD3D11PresentOld(IDXGISwapChain* pSwapChain)
{
	if (GUI::Render.NewFrame(pSwapChain))
	{

		Vector2 kek = GUI::Render.CanvasSize();
		Vars::Global::ScreenWidth = kek.x;
		Vars::Global::ScreenHigh = kek.y;

		if (Vars::AimBot::BodyAim) Vars::Global::BoneToAim = 1;
		else Vars::Global::BoneToAim = BoneList(neck);

		if (Vars::AimBot::DrawFov)
			DrawFOV();

		if (Vars::AimBot::Crosshair)
			Crosshair();

		EntityZaLoop();

		if (GUI::Visible)
		{
			if (Vars::AimBot::Fov > (kek.y - 3)) Vars::AimBot::Fov = (kek.y - 3);
			D2D1::ColorF FillClr = { 220, 20, 60 };

			GUI::CurWnd.Pos.y -= 15.f;
			float CurPos = GUI::CurWnd.Pos.y;
			const wchar_t* pos[5] =
			{
				L" A̲i̲m̲B̲o̲t̲", L" V̲i̲s̲u̲a̲l̲s̲", L"W̲e̲a̲p̲o̲n̲", L"   M̲i̲s̲c̲", L"  Other"
			};
			for (int i = 0; i < 5 && false; i++)
			{
				D2D1::ColorF TextClr = { 99, 0, 0, 1.0f };
				D2D1::ColorF FillClr = { 99, 0, 0, 1.0f };
				if (i == Vars::Global::CurPage)
				{
				}
				else
				{
				}
				Render.String({ CurWnd.Pos.x + 20.f, CurPos }, pos[i], TextClr, true);
				if (IO.LKM == Clicked && InRect({ CurWnd.Pos.x + 20.f, CurPos }, { 80.f, 30.f }, IO.MousePos) && InputMgr(L"Tab", true)) Vars::Global::CurPage = i;
				CurPos += 40.f;

			}

			GUI::End();
		}

		GUI::Render.EndFrame();
	}
	GUI::Render.EndFrame();
	Vars::Global::PresentDelta = 0;
}

__declspec(noinline) void StartRender()
{

	{
		if (!GetModBase(L"DiscordHook64.dll"))
		{
			if (MessageBoxA(nullptr, "Нюхай Бебру Далбоёп,Запусти Оверлей", "иди нахуй", MB_OK) == 1)
			{
				exit(1);
			}
		}
	}


	DWORD64 dwX64PresentOrig = NULL; DWORD64 dwX64ResizeBuffers = NULL;
	//hWnd = FC(user32, FindWindowA, StrA("UnityWndClass"), NULL);
	// GUI::NextWndProc = (WNDPROC)FC(user32, SetWindowLongPtrW, hWnd, GWLP_WNDPROC, (LONG_PTR)GUI::WndProc);
	origPresent = (PDWORD64)RVA(FindPattern((PBYTE)"\xFF\x15\x00\x00\x00\x00\x8B\xD8\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xEB\x10", StrA("xx????xxx????x????xx"), StrW(L"DiscordHook64.dll")), 6);
	dwX64PresentOrig = *origPresent;
	oPresent = (DX11PresentFn)(*origPresent);
	*origPresent = (DWORD_PTR)(&hookD3D11Present);

	origResize = (PDWORD64)RVA(FindPattern((PBYTE)"\x44\x8B\xCB\x44\x8B\xC7", StrA("xxxxxx"), StrW(L"DiscordHook64.dll")) + 0x1F, 7);
	dwX64ResizeBuffers = *origResize;
	oResize = (DX11ResizeFn)(*origResize);
	*origResize = (DWORD_PTR)(&Resize);

	//CreateThreadSafe(EntityHandler);
	//CreateThreadSafe(CheatFunc);
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved)
{
	if (dwCallReason == DLL_PROCESS_ATTACH)
	{
#if AllocCons == 1
#endif
		InitHook();
		//DischanSDK::ProcessFirstCheck(lpReserved);
		g_hModule = hModule;
		StartRender();

	}

	return TRUE;
}
