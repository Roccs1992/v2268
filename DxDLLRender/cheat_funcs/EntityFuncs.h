#include "AimFuncs.h"
#include "Hook/MenuHook.h"


bool mfound = false;
void FindMatrix() {
	static DWORD64 dwGameObjectManager = 0;
	if (!dwGameObjectManager)
		dwGameObjectManager = RVA(FindPattern((PBYTE)"\x48\x8B\x15\x00\x00\x00\x00\x66\x39", "xxx????xx", L"UnityPlayer.dll"), 7);

	DWORD64 ObjMgr = safe_read(dwGameObjectManager, DWORD64);
	if (!ObjMgr) return;
	UINT64 end = safe_read(ObjMgr, UINT64);
	for (UINT64 Obj = safe_read(ObjMgr + 0x8, UINT64); (Obj && (Obj != end)); Obj = safe_read(Obj + 0x8, UINT64))
	{
		UINT64 GameObject = safe_read(Obj + 0x10, UINT64);
		WORD Tag = safe_read(GameObject + 0x54, WORD);
		if (Tag == 5)
		{
			UINT64 ObjClass = safe_read(GameObject + 0x30, UINT64);
			UINT64	Entity = safe_read(ObjClass + 0x18, UINT64);
			LocalPlayer.pViewMatrix = (Matrix4x4*)(Entity + 0xDC);
			printf(StrA("Found matrix!\n"));
			mfound = true;
			return;
		}
		else if (Tag == 20011 && Vars::Misc::AlwaysDay)
		{
			UINT64 ObjClass = safe_read(GameObject + 0x30, UINT64);
			UINT64	Entity = safe_read(ObjClass + 0x18, UINT64);
			DWORD64 Dome = safe_read(Entity + 0x28, DWORD64);
			DWORD64 TodCycle = safe_read(Dome + 0x38, DWORD64);
			safe_write(TodCycle + 0x10, 13.37f, float);
		}
		else if (Tag == 20011 && Vars::Misc::CustomTime) {
			UINT64 ObjClass = safe_read(GameObject + 0x30, UINT64);
			UINT64	Entity = safe_read(ObjClass + 0x18, UINT64);
			DWORD64 Dome = safe_read(Entity + 0x28, DWORD64);
			DWORD64 TodCycle = safe_read(Dome + 0x38, DWORD64);
			safe_write(TodCycle + 0x10, Vars::Misc::Time, float);
		}

	}
}

/*inline void ThirdPerson()//work
{
	if (Vars::Misc::IsThirdPerson && LocalPlayer.BasePlayer)
	{
		auto Flag = safe_read(reinterpret_cast<uintptr_t>(LocalPlayer.BasePlayer) + oPlayerFlags, uintptr_t);
		safe_write(reinterpret_cast<uintptr_t>(LocalPlayer.BasePlayer) + oPlayerFlags, Flag |= 1024, uintptr_t);
	}
}

inline void FakeLag()
{
	if (Vars::Misc::FakeLag)
	{
		safe_write(reinterpret_cast<uintptr_t>(LocalPlayer.BasePlayer) + oClientTickInterval, 0.4f, float);
	}
	else
	{
		safe_write(reinterpret_cast<uintptr_t>(LocalPlayer.BasePlayer) + oClientTickInterval, 0.05f, float);
	}
}*/

void EntityZaLoop()
{
	bool LP_isValid = false;
	BasePlayer* SilentTargetedPlayer = 0;
	BasePlayer* AimEntity = nullptr;
	float FOV = Vars::AimBot::Fov, CurFOV;
	float flSilentSmallestFOV = 999.0f;
	BasePlayer* TargetAimBotPlayer = nullptr;
	if (!LocalPlayer.pViewMatrix || !mfound)
	{
		FindMatrix();
	}

	class BaseProjectile
	{
	public:
		char pad_0x0[0x158]; //0x0
		bool isDeployed; //0x158
		char pad_0x159[0x3]; //0x159
		float hostileScore; //0x15c
		char pad_0x164[0x4]; //0x16
		float deployDelay; //0x170
		float repeatDelay; //0x174
		float nextAttackTime; //0x178
		float lastTickTime; //0x17c
		float nextTickTime; //0x180
		float timeSinceDeploy; //0x184
		bool catchup; //0x188
		char pad_0x189[0x7]; //0x189
		float damageScale; //0x1c8
		float distanceScale; //0x1cc
		float projectileVelocityScale; //0x1d0
		bool automatic; //0x1d4
		char pad_0x1d5[0x3]; //0x1d5
		float reloadTime; //0x1d8
		bool canUnloadAmmo; //0x1dc
		char pad_0x1dd[0x3]; //0x1dd
		float aimSway; //0x1e0
		float aimSwaySpeed; //0x1e4
		float aimCone; //0x1e8
		float hipAimCone; //0x1ec
		bool hasADS; //0x1f0
		bool noAimingWhileCycling; //0x1f1
		bool manualCycle; //0x1f2
		bool needsCycle; //0x1f3
		bool isCycling; //0x1f4
		bool aiming; //0x1f5
		char pad_0x1f6[0x2]; //0x1f6
		float nextReloadTime; //0x1f8
		bool isReloading; //0x1fc
		char pad_0x1fd[0x3]; //0x1fd
		float reloadPressTime; //0x200
	};

	DWORD64 BaseNetworkable;
	if (isSteam == -1) BaseNetworkable = safe_read(GetModBase(StrW(L"GameAssembly.dll")) + g_BN, DWORD64);
	else BaseNetworkable = safe_read(GetModBase(StrW(L"GameAssembly.dll")) + g_BN_Steam, DWORD64);
	DWORD64 EntityRealm = safe_read(BaseNetworkable + 0xB8, DWORD64);
	DWORD64 ClientEntities = safe_read(EntityRealm, DWORD64);
	DWORD64 ClientEntities_list = safe_read(ClientEntities + 0x10, DWORD64);
	DWORD64 ClientEntities_values = safe_read(ClientEntities_list + 0x28, DWORD64);
	if (!ClientEntities_values) return;
	int EntityCount = safe_read(ClientEntities_values + 0x10, int);
	DWORD64 EntityBuffer = safe_read(ClientEntities_values + 0x18, DWORD64);



	for (int i = 0; i <= EntityCount; i++)
	{
		DWORD64 Entity = safe_read(EntityBuffer + 0x20 + (i * 0x8), DWORD64);
		if (Entity <= 100000) continue;
		DWORD64 Object = safe_read(Entity + 0x10, DWORD64);
		if (Object <= 100000) continue;
		DWORD64 ObjectClass = safe_read(Object + 0x30, DWORD64);
		if (ObjectClass <= 100000) continue;
		pUncStr name = safe_read(ObjectClass + 0x60, pUncStr);
		if (!name) continue;
		char* buff = name->stub;
		if (strstr(buff, StrA("Local"))) {
			BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
			if (!safe_read(Player + 0x50, DWORD64) || safe_read(Player + 0x18, bool) || !safe_read(Player + oPlayerModel, DWORD64)) continue;
			if (Player != LocalPlayer.BasePlayer /*lp changed*/) {
				printf("LocalPlayer %lld\n", Player->GetSteamID());
				mfound = false;
			}
			Vector3 pos = Player->GetPosition();
			//printf("lp pos %f %f %f\n", pos.x, pos.y, pos.z);
			LocalPlayer.BasePlayer = Player;
			LP_isValid = true;

		}
		else if ((strstr(buff, StrA("npc")) || strstr(buff, StrA("scientist"))) && (!strstr(buff, StrA("prop")) && !strstr(buff, StrA("corpse")) && !strstr(buff, StrA("turret")) && !strstr(buff, StrA("cargo")))) {
			BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
			if (!safe_read(Player + 0x50, DWORD64) || safe_read(Player + 0x18, bool) || !safe_read(Player + oPlayerModel, DWORD64)) continue;
			if (!Player->IsValid()) continue;
			if (!Vars::PlayerEsp::IgnoreNpc) ESP(Player, LocalPlayer.BasePlayer, true);
			if (Vars::AimBot::IgnoreNpc) continue;

			CurFOV = GetFov(Player, BoneList(Vars::Global::BoneToAim));
			
			if (CurFOV <= Vars::AimBot::SilentFov) {
				TargetSilentPlayer = (uintptr_t)Player;
			}
			//cut distance
			if (Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetBoneByID(head), Player->GetBoneByID(head)) > Vars::AimBot::Range)
				continue;

			//calc visible & low fov
			float CurFOV;
			if (Player->IsVisible() && (FOV > (CurFOV = GetFov(Player, BoneList(Vars::Global::BoneToAim))))) {
				if (CurFOV <= Vars::AimBot::Fov) {
					FOV = CurFOV; AimEntity = Player;
				}
			}
		}

		else if (strstr(buff, StrA("player.prefab")) && (!strstr(buff, StrA("prop")) && !strstr(buff, StrA("corpse")))) {
			BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
			if (!safe_read(Player + 0x50, DWORD64) || safe_read(Player + 0x18, bool) || !safe_read(Player + oPlayerModel, DWORD64)) continue;
			if (!Player->IsValid()) continue;
			bool ex = false;

			//printf("Esp on %lld\n", Player->GetSteamID());
			ESP(Player, LocalPlayer.BasePlayer);
			if (Vars::PlayerEsp::IgnoreSleepers && Player->HasFlags(16)) continue;
			if (Vars::PlayerEsp::PlayerWounded && Player->HasFlags(16)) continue;
			//find target
			if (Vars::AimBot::IgnoreTeam) {
				DWORD64 EntitySID = Player->GetSteamID();
				for (int j = 0; !ex && j < LocalPlayer.BasePlayer->GetTeamSize(); j++) {
					DWORD64 SlotSID = LocalPlayer.BasePlayer->IsTeamMate(j);
					if (SlotSID == EntitySID) ex = true;
				}
			}

			if (ex) continue;
			CurFOV = GetFov(Player, BoneList(Vars::Global::BoneToAim));

			if (CurFOV <= Vars::AimBot::SilentFov) {
				TargetSilentPlayer = (uintptr_t)Player;
			}
			//cut distance
			if (Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetBoneByID(head), Player->GetBoneByID(head)) > Vars::AimBot::Range)
				continue;

			//calc visible & low fov
			float CurFOV;
			if (Player->IsVisible() && (FOV > (CurFOV = GetFov(Player, BoneList(Vars::Global::BoneToAim))))) {
				if (CurFOV <= Vars::AimBot::Fov) {
					FOV = CurFOV; AimEntity = Player;
				}
			}

		}

		else if (Vars::Visuals::Stone && strstr(buff, "stone-ore.prefab")) {//stone-ore.prefab
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Stone");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::StoneColor));
			}
		}

		else if (Vars::Visuals::Sulphur && strstr(buff, "sulfur-ore.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Sulfur");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::SulphurColor));
			}
		}

		else if (Vars::Visuals::Metal && strstr(buff, "metal-ore.prefab")) {//metal-ore.prefab
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Metal");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::MetalColor));
			}
		}

		//pick-up

		else if (Vars::Visuals::PickUp_Metal && strstr(buff, "metal-collectable.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Pickup Metal");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::PickUp_MetalColor));
			}
		}

		else if (Vars::Visuals::PickUp_Stone && strstr(buff, "stone-collectable.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Pickup Stone");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::PickUp_StoneColor));
			}
		}

		else if (Vars::Visuals::PickUp_Sulfur && strstr(buff, "sulfur-collectable.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Pickup Sulfur");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::PickUp_SulfurColor));
			}
		}

		else if (Vars::Visuals::PickUp_Wood && strstr(buff, "wood-collectable.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Pickup Wood");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::PickUp_WoodColor));
			}
		}

		else if (Vars::Visuals::Stash && strstr(buff, "small_stash_deployed.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Stash");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::StashColor));
			}
		}

		else if (Vars::Visuals::Hemp && strstr(buff, "hemp-collectable.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Hemp");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::HempColor));
			}
		}

		else if (Vars::Visuals::Minicopter && strstr(buff, "minicopter.entity.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Minicopter");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::MinicopterColor));
			}
		}

		else if (Vars::Visuals::Turret && strstr(buff, "autoturret_deployed.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Turret");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::TurretColor));
			}
		}

		else if (Vars::Visuals::Corpse && strstr(buff, "player_corpse.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Corpse");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::CorpseColor));
			}
		}

		else if (Vars::Visuals::Backpack && strstr(buff, "item_drop_backpack.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Backpack");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::BackpackColor));
			}
		}

		else if (Vars::Visuals::Guntrap && strstr(buff, "guntrap.deployed.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Guntrap");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::GuntrapColor));
			}
		}

		else if (Vars::Visuals::Mushrooms && strstr(buff, "mushroom-cluster-5.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Mushroom"); //gab loot
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::MushroomsColor));
			}
		}

		else if (Vars::Visuals::bagloot && strstr(buff, "item_drop.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Bagloot");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::BagLootColor));
			}
		}

		else if (Vars::Visuals::Airdrop && strstr(buff, "supply_drop.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
			float distance = (int)Math::Calc3D_Dist(pos, localPos);
			float multiplier = 1.0f - distance / 400.0f;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				std::wstring name(L"Airdrop");
				name.append(std::to_wstring((int)distance));
				GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::AirdropColor));
			}
		}

		else if (Vars::Visuals::Cupboard && strstr(buff, "cupboard.tool.deployed.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Cupboard");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::CupboardColor));
		}
		}

		else if (Vars::Visuals::LWB && strstr(buff, "box.wooden.large.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Large Wooden Box");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::LWBColor));
		}
		}
		///////////////////
		else if (Vars::Visuals::campfire && (strstr(buff, "hobobarrel_static.prefab") || strstr(buff, "campfire.prefab"))) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Camp Fire");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::CampFireCol));
		}
		}

		else if (Vars::Visuals::Barrel && (strstr(buff, "oil_barrel.prefab") || strstr(buff, "loot_barrel_1.prefab") || strstr(buff, "loot_barrel_2.prefab"))) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Barrel");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::BarrelCol));
		}
		}

		else if (Vars::Visuals::Cargo && strstr(buff, "cargomarker.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 1000.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Cargo Ship");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::CargoCol));
		}
		}

		else if (Vars::Visuals::Furnace && strstr(buff, "furnace.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Furnace");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::FurnaceCol));
		}
		}

		else if (Vars::Visuals::PatrolHeli && strstr(buff, "assets/prefabs/npc/patrol helicopter/patrolhelicopter.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				int distation = (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetBoneByID(head), pos);
				wchar_t text[0x100];
				_swprintf(text, L"Chopper", distation);
				GUI::Render.String(Pos, text, FLOAT4TOD3DCOLOR2(Vars::Color::PatrolHeliCol));
			}
		}

		else if (Vars::Animals::Wolf && strstr(buff, "wolf.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Wolf");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::WolfColor));
		}
		}

		else if (Vars::Animals::Deer && strstr(buff, "deer.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Deer");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::DeerColor));
		}
		}

		else if (Vars::Animals::Horse && strstr(buff, "horse.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / Vars::Distance::Animals;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Horse");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::HorseColor));
		}
		}

		else if (Vars::Animals::Chicken && strstr(buff, "chicken.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / Vars::Distance::Animals;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Chicken");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::ChickenColor));
		}
		}

		else if (Vars::Animals::Boar && strstr(buff, "boar.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / Vars::Distance::Animals;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Boar");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::BoarColor));
		}
		}

		else if (Vars::Animals::Bears && strstr(buff, "bear.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / Vars::Distance::Animals;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Bea");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::BearsColor));
		}
		}

		else if (Vars::technique::cars && (strstr(buff, "assets/content/vehicles/modularcar/2module_car_spawned") || strstr(buff, "assets/content/vehicles/modularcar/3module_car_spawned") || strstr(buff, "assets/content/vehicles/modularcar/4module_car_spawned"))) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Cars");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::CarsCol));
		}
		}

		else if (Vars::technique::Rowboat && strstr(buff, "rowboat.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Rowboat");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::RowboatCol));
		}
		}

		else if (Vars::technique::RHIB && strstr(buff, "rhib.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"RHIB");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::RHIBCol));
		}
		}

		else if (Vars::technique::Kayak && strstr(buff, "kayak.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); 
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Kayak");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::KayakCol));
		}
		}

		else if (Vars::Visuals::Pumpkin && (strstr(buff, "pumpkin-collectable.prefab") || strstr(buff, "pumpkin.entity.prefab"))) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Pumpkin");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::PumpkinColor));
		}
		}

		else if (Vars::Visuals::Potato && strstr(buff, "potato-collectable.prefab")) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); 
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Potato");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::PotatoColor));
		}
		}

		else if (Vars::Visuals::Corn && (strstr(buff, "corn-collectable.prefab") || strstr(buff, "corn.entity.prefab"))) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Corn");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::CornColor));
		}
		}

		else if (Vars::Visuals::Berry && (strstr(buff, "berry-green-collectable.prefab") || strstr(buff, "berry-red-collectable.prefab") || strstr(buff, "berry-blue-collectable.prefab") || strstr(buff, "berry-white-collectable.prefab"))) {
		DWORD64 Res = safe_read(Object + 0x28, DWORD64);
		DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
		DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
		DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
		Vector3 pos = safe_read(Vec + 0x90, Vector3);
		Vector2 Pos;
		Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(jaw);
		float distance = (int)Math::Calc3D_Dist(pos, localPos);
		float multiplier = 1.0f - distance / 400.0f;
		if (LocalPlayer.WorldToScreen(pos, Pos)) {
			std::wstring name(L"Berrys");
			name.append(std::to_wstring((int)distance));
			GUI::Render.String(Pos, name.c_str(), FLOAT4TOD3DCOLOR2(Vars::Color::BerryColor));
		}
		}

	}

	if (LP_isValid)
	{
		FakeLag();
		ThirdPerson();
		WeaponPatch();
		Aim(AimEntity);
		Misc();

		
	}

	//LP not valid
	else LocalPlayer.BasePlayer = nullptr;




	static DWORD64 dwGameObjectManager = 0;
	if (!dwGameObjectManager)
		dwGameObjectManager = RVA(FindPattern((PBYTE)"\x48\x8B\x15\x00\x00\x00\x00\x66\x39", "xxx????xx", L"UnityPlayer.dll"), 7);

	DWORD64 ObjMgr = safe_read(dwGameObjectManager, DWORD64); if (!ObjMgr) return;




	for (DWORD64 Obj = safe_read(ObjMgr + 0x8, DWORD64); (Obj && (Obj != safe_read(ObjMgr, DWORD64))); Obj = safe_read(Obj + 0x8, DWORD64))
	{
		DWORD64 GameObject = safe_read(Obj + 0x10, DWORD64);
		WORD Tag = safe_read(GameObject + 0x54, WORD);

		if (Tag == 6 || Tag == 5 || Tag == 20011)
		{
			DWORD64 ObjClass = safe_read(GameObject + 0x30, DWORD64);
			DWORD64	Entity = safe_read(ObjClass + 0x18, DWORD64);

			//entity
			if (Tag == 6)
			{
				
				//entity
				if (safe_read(safe_read(GameObject + 0x60, DWORD64), DWORD64) != 0x616C506C61636F4C)
				{

				}
			
				else {
					
				}
			}

			//camera
			else if (Tag == 5)
				LocalPlayer.pViewMatrix = (Matrix4x4*)(Entity + 0xDC);

			//sky
			else if (Tag == 20011 && Vars::Misc::AlwaysDay) {
				DWORD64 Dome = safe_read(Entity + 0x28, DWORD64);
				DWORD64 TodCycle = safe_read(Dome + 0x38, DWORD64);
				safe_write(TodCycle + 0x10, 13.37f, float);
			}
			else if (Tag == 20011 && Vars::Misc::CustomTime) {
				DWORD64 Dome = safe_read(Entity + 0x28, DWORD64);
				DWORD64 TodCycle = safe_read(Dome + 0x38, DWORD64);
				safe_write(TodCycle + 0x10, Vars::Misc::Time, float);
			}

		}

		//goto next entity
	NextEnt: continue;
	}

	//GET MEMES
	if (LP_isValid)
	{
		WeaponPatch();
		Aim(AimEntity);
		Misc();

		
	}

	//LP not valid
	else LocalPlayer.BasePlayer = nullptr;
}
