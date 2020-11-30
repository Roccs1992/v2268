#pragma once
#include "MinHook.h"
#include "buffer.h"


#define oAimconedirection 0x781AD0 //public static Vector3 GetModifiedAimConeDirection(float aimCone, Vector3 inputVec, bool anywhereInside = True) { }
#define oSetFlying 0x179B520 //public void set_flying(bool value) { } 
#define oCanAttack 0x28DE70 // public bool CanAttack()
#define oDickHard 0x28DE70 // public bool CanAttack()
#define oBlockSprint 0x28D1E0 // public virtual void BlockSprint(float duration = 0,2) { }
#define oGetSpreadScalar 0x3A45D0 // public float GetSpreadScalar() { }
#define oGetIndexedSpreadScalar 0x3A4500   // public float GetIndexedSpreadScalar(int shotIndex, int maxShots) { }
#define oGetSpeed 0x297A30    // 	public float GetSpeed(float running, float ducking) { }
#define oCreateProjectile 0x27F8A0  // private Projectile CreateProjectile(string prefabPath, Vector3 pos, Vector3 forward, Vector3 velocity) { }
#define oSendProjectileAttack 0x2A5390 // 	public void SendProjectileAttack(PlayerProjectileAttack attack) { }
#define oAntiAim 0x2A4CD0 //internal void SendClientTick() { }
#define oClientInput 0x27DBC0  //public virtual void ClientInput(InputState state, ModelState modelState) { }

#define oHitID 0x2C// public uint hitID;
#define oHitPartID 0x64// public uint hitPartID;
#define oHitBone 0x30// public uint hitBone;  ? //HitInfo 
#define oHitMaterialID 0x68 // public uint hitMaterialID;
#define oPlayerAttack 0x18 // public PlayerAttack playerAttack;
#define oAttack 0x18 // public Attack attack;
#define oHitNormalWorld 0x4C // public Vector3 hitNormalWorld; ? //public class Attack : IDisposable, Pool.IPooled, IProto 
#define oNetworkable 0x50// public Networkable net;
#define oNetworkId 0x10//public uint ID;



typedef void(__fastcall* SendProjectileAttack_fn)(void*, void*);
SendProjectileAttack_fn Orig_SendProjectileAttack{ };

typedef void(__fastcall* SetFlying_fn)(void*, void*);
SetFlying_fn original_setflying{};

__declspec(selectany) uintptr_t TargetSilentPlayer = NULL;

typedef bool(__fastcall* Can_Attack)(void*, void*);
Can_Attack Orig_CanAttack;

typedef bool(__fastcall* DickHard)(void*, void*);
DickHard Orig_Dick_Hard;

typedef float(__fastcall* GetSpeed)(float* a1, float* a2);
GetSpeed Orig_GetSpeed;

typedef void(__fastcall* BlockSprint)(void*);
BlockSprint Orig_BlockSprint;

typedef void(__fastcall* FastHeal_)(void* a1, void* a2);
FastHeal_ Orig_Heal;

typedef uintptr_t(__fastcall* CreateProjectile)(void*, void*, Vector3, Vector3, Vector3);
CreateProjectile Orig_CreateProjectile{ };

typedef float(__fastcall* GetIndexedSpreadScalar)(int*, int*);
GetIndexedSpreadScalar Orig_GetIndexedSpreadScalar{ };

typedef float(__fastcall* GetSpreadScalar)(int*, int*);
GetSpreadScalar Orig_GetSpreadScalar{ };


typedef void(__fastcall* AntiAim)(void*);
AntiAim original_sendclienttick;

typedef void(__stdcall* ClientInputt)(DWORD64, DWORD64);
ClientInputt original_clientinput;

typedef Vector3(__fastcall* aimconedirection)(float, Vector3, bool);
aimconedirection original_aimconedirection{};

void __fastcall ClientInput(DWORD64 baseplayah, DWORD64 ModelState) {
	typedef void(__stdcall* ClientInput)(DWORD64, DWORD64);
	((ClientInput)original_clientinput)(baseplayah, ModelState);
	if (Vars::Misc::SilentWalk)
		LocalPlayer.BasePlayer->SetRemoveFlag(4);
}

inline void __fastcall hook_sendclienttick(void* self)
{
	if (Vars::Misc::AntiAim)
	{
		auto input = safe_read(self + oPlayerInput, uintptr_t);  //Input
		if (!input)
		{
			//std::cout << "Failed to get input!" << std::endl;
			return original_sendclienttick(self);
		}

		auto state = safe_read(input + 0x20, uintptr_t);
		if (!state)
		{
			//std::cout << "Failed to get state!" << std::endl;
			return original_sendclienttick(self);
		}

		auto current = safe_read(state + 0x10, uintptr_t);
		if (!current)
		{
			//std::cout << "Failed to get current!" << std::endl;
			return original_sendclienttick(self);
		}
		safe_write(current + 0x18, Vector3(100, rand() % 999 + -999, rand() % 999 + -999), Vector3);
	}
	return original_sendclienttick(self);
}


inline float __fastcall Fake_GetSpeed(float* a1, float* a2)
{
	if (Vars::Misc::SpeedHack)
	{
		return Vars::Misc::SpeedValue;
	}
	return  Orig_GetSpeed(a1, a2);
}

inline void ThirdPerson()//work
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
}

/*inline uintptr_t __fastcall Fake_CreateProjectile(void* BaseProjectile, void* prefab_pathptr, Vector3 pos, Vector3 forward, Vector3 velocity)
{
	const uintptr_t Projectile = Orig_CreateProjectile(BaseProjectile, prefab_pathptr, pos, forward, velocity);
	if (Vars::Misc::HookFatBullet)
	{
		safe_write(Projectile + 0x2C , 1.f, float);
	}
	return Projectile;
}*/


inline bool __fastcall Fake_DickHard(void* a1, void* a2)
{
	if (Vars::Misc::SilentWalk)
		return true;

	return Orig_Dick_Hard(a1, a2);
}

inline bool __fastcall Fake_Can_Attack(void* a1, void* a2)
{
	if (Vars::AimBot::JumpShot)
		return true;

	return Orig_CanAttack(a1, a2);
}

inline void __fastcall Fake_BlockSprint(void* a1)
{
	if (Vars::AimBot::JumpShot)
		return;

	return Orig_BlockSprint(a1);
}

/*bool IsUsing_FastHeal = true;
float Use_Last_FastHeal;

class Str {
	char zpad[0x10];
public:
	int size;
	wchar_t str[64 + 1];
	Str(const wchar_t* st) {
		size = min(wcslen((wchar_t*)st), 64);
		for (int i = 0; i < size; i++) {
			str[i] = st[i];
		}
		str[size] = 0;
	}
};*/

/*inline void ServerRPC(uintptr_t Entity, const wchar_t* ServerMethod)
{
	typedef void(__stdcall* ServerRPC)(DWORD64, Str);
	auto* RPC = reinterpret_cast<ServerRPC>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oFastHeal));
	RPC(Entity, Str(ServerMethod));
}*/


//auto* weapon = LocalPlayer.BasePlayer->GetActiveWeapon();
//inline void __fastcall FastHeal(void* a1, void* a2) {
//	if (Vars::Misc::FastHeal && (weapon->GetID() == 1079279582 || weapon->GetID() == -2072273936) && LocalPlayer.BasePlayer->GetHealth() < 95.f) // syringe | bandage
//	{
//		DWORD64 Held = safe_read(weapon + 0x98, DWORD64);
//		bool deployed = safe_read(Held + 0x188, bool);
//		auto CurTime = LocalPlayer.BasePlayer->GetTick();
//
//		if (deployed && CurTime > Use_Last_FastHeal + 0.8f)
//		{
//			if (IsUsing_FastHeal)
//			{
//				ServerRPC(static_cast<uintptr_t>(Held), L"UseSelf");
//			}
//			else
//				CurTime += 0.3f;
//			Use_Last_FastHeal = CurTime;
//			IsUsing_FastHeal = true;
//		}
//		else if (!deployed)
//		{
//			IsUsing_FastHeal = false;
//		}
//	}
//	else
//	{
//		IsUsing_FastHeal = false;
//		return Orig_Heal(a1, a2);
//	}
//}


//0x347EE0


inline void __fastcall Fake_SendProjectileAttack(void* a1, void* a2)
{
	if (Vars::AimBot::SilentAim)
	{
		auto* TargetPlayer = reinterpret_cast<BasePlayer*>(TargetSilentPlayer);
		if (!(TargetPlayer->GetBoneByID(spine1).x == 0 && TargetPlayer->GetBoneByID(spine1).y == 0 && TargetPlayer->GetBoneByID(spine1).z == 0))
		{

			if (TargetSilentPlayer != NULL)
			{

				const auto PlayerAttack = safe_read(reinterpret_cast<uintptr_t>(a2) + oPlayerAttack, uintptr_t);
				const auto Attack = safe_read(PlayerAttack + oAttack, uintptr_t);
				if (!Vars::AimBot::SilentBody)
					safe_write(Attack + oHitBone, 698017942, uint32_t);
				if (Vars::AimBot::SilentBody)
					safe_write(Attack + oHitBone, 827230707u, uint32_t);
				safe_write(Attack + oHitPartID, 16144115, uint32_t);
				safe_write(Attack + oHitID, safe_read(safe_read(TargetSilentPlayer + oNetworkable, uintptr_t) + oNetworkId, uint32_t), uint32_t);

				if (Vars::AimBot::ThroughWall)
				{
					safe_write(Attack + oHitNormalWorld, Vector3(0.f, -1000.f, 0.f) * -10000.f, Vector3);
				}


			}
		}
	}
	return Orig_SendProjectileAttack(a1, a2);
}


inline float __fastcall Fake_GetIndexedSpreadScalar(int* a1, int* a2)
{
	if (Vars::Misc::ShotGunTochka)
	{
		return 0.0f;
	}
	return  Orig_GetIndexedSpreadScalar(a1, a2);
}

inline float __fastcall Fake_GetSpreadScalar(int* a1, int* a2)
{
	if (Vars::Misc::ShotGunTochka)
	{
		return 0.0f;
	}
	return  Orig_GetSpreadScalar(a1, a2);
}

void __fastcall SetFlying(void* a1, void* a2) {
	if (Vars::Misc::FakeAdmin)
		return;
}

inline void CreateHook(void* Function, void** Original, void* Detour, bool autoEnable = true)
{
	if (MH_Initialize() != MH_OK && MH_Initialize() != MH_ERROR_ALREADY_INITIALIZED)
	{
		std::cout << "Failed to initialize Hook" << std::endl;
		return;
	}
	MH_CreateHook(Function, Detour, Original);
	if (autoEnable)
		MH_EnableHook(Function);
}

inline void InitHook()
{
	if (MH_Initialize() != MH_OK && MH_Initialize() != MH_ERROR_ALREADY_INITIALIZED)
	{
		std::cout << "Failed to initialize Hook" << std::endl;
		return;
	}
	//CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oFastHeal)), reinterpret_cast<void**>(&original_setflying), FastHeal);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oSetFlying)), reinterpret_cast<void**>(&original_setflying), SetFlying);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oSendProjectileAttack)), reinterpret_cast<void**>(&Orig_SendProjectileAttack), Fake_SendProjectileAttack);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oCanAttack)), reinterpret_cast<void**>(&Orig_CanAttack), Fake_Can_Attack);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oBlockSprint)), reinterpret_cast<void**>(&Orig_BlockSprint), Fake_BlockSprint);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oGetSpeed)), reinterpret_cast<void**>(&Orig_GetSpeed), Fake_GetSpeed);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oGetIndexedSpreadScalar)), reinterpret_cast<void**>(&Orig_GetIndexedSpreadScalar), Fake_GetIndexedSpreadScalar);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oGetSpreadScalar)), reinterpret_cast<void**>(&Orig_GetSpreadScalar), Fake_GetSpreadScalar);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oAntiAim)), reinterpret_cast<void**>(&original_sendclienttick), hook_sendclienttick);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oClientInput)), reinterpret_cast<void**>(&original_clientinput), ClientInput);
	//CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oTraceAll)), reinterpret_cast<void**>(&original_clientinput), TraceAll);
	CreateHook(reinterpret_cast<void*>(static_cast<uintptr_t>(GetModBase(L"GameAssembly.dll") + oDickHard)), reinterpret_cast<void**>(&Orig_Dick_Hard), Fake_DickHard);
}