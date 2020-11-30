#include "Hook/MenuHook.h"

void WeaponPatch()
{
	USHORT Hash = 0;
	WeaponData* Weapon = LocalPlayer.BasePlayer->GetActiveWeapon();
	if (Weapon) Hash = Weapon->GetNameHash();

	switch (Hash)
	{
		//nullptr
	case 0: return;

		//Weapon
	case 0x5A9F:
		Weapon->SuperBow();
		Weapon->FatBullet();
		Weapon->Compund();
		break;

	case 0x6B1:
	case 0x74F5:
	case 0x67DA:
	case 0x3146:
	case 0x79C4:
	case 0x573C:
	case 0xB32A:
	case 0xC196:
		Weapon->SetAutomatic();
		Weapon->FatBullet();
		Weapon->Compund();
	case 0x7983:
	case 0xE5EA:
	case 0x77E9:
	case 0xE97D:
	case 0xC2DD:
	case 0x52F7:
	case 0xFDC3:
	case 0x62E5:
	case 0x6A09:
	case 0x3511:
	case 0xCFA1:
	case 0xF87E:
		Weapon->AntiSpread();
		Weapon->NoRecoil();
		Weapon->FatBullet();
		Weapon->Compund();

	case 0xB0A0:
	case 0x435E:
	{

		if (Hash == 0x435E)
		{
			Weapon->FatBullet();
			Weapon->SuperEoka();
			Weapon->Compund();
		}
	} break;


	case 0xF3B9:
		Weapon->Compund();
		break;
	case 0xCBD8:
	case 0xAB85:
	case 0xFB2B:
	case 0x6104:
	case 0xC859:
	case 0x96D3:
	case 0x7340:
	case 0xC68B:
	case 0x2F12:
	case 0x65B4:
	case 0x1F2D:
	case 0x2658:
	case 0xAC02:
	case 0x282A:
	case 0x8151:
	case 0xFACE:
	case 0x7296:
	case 0x4B0A:
	case 0xCD:
	case 0x2333:
		if (Vars::Misc::LongHand) {
			Weapon->FatHand();
			Weapon->LongHand();
			Weapon->RunOnHit();
			Weapon->Compund();
			break;
		}
	}
}


float GetFov(BasePlayer* Entity, BoneList Bone) {
	Vector2 ScreenPos;
	if (!LocalPlayer.WorldToScreen(Entity->GetBoneByID(Bone), ScreenPos)) return 1000.f;
	return Math::Calc2D_Dist(Vector2(Vars::Global::ScreenWidth / 2, Vars::Global::ScreenHigh / 2), ScreenPos);
}


float GetGravity(int ammoid) {
	switch (ammoid) {
	case 14241751: //fire arrow
		return 1.f;
	case -1234735557: //arrow normal
		return 0.75f;
	case 215754713: //arrow bone
		return 0.75f;
	case -1023065463: //hv arrow
		return 0.5f;
	case -2097376851: //nails
		return 0.75f;
	case -1321651331: //5.56 exp
		return 1.25f;
	default:
		return 1.f;
	}
}

float GetBulletSpeed(Weapon tar, int ammo)
{
	if (ammo == 0) return tar.ammo[0].speed; //melee
	for (Ammo am : tar.ammo) {
		for (int id : am.id) {
			if (id == ammo) {
				printf("returned %f\n", am.speed);
				return am.speed;
			}
		}
		if (am.id[0] == 0) return am.speed;
	}
	return 250.f;
}



Vector3 Prediction(const Vector3& LP_Pos, BasePlayer* Player, BoneList Bone)
{
	WeaponData* active = LocalPlayer.BasePlayer->GetActiveWeapon();
	Weapon tar = active->Info();
	int ammo = active->LoadedAmmo();
	printf("loaded ammo %d   ", ammo);
	Vector3 BonePos = Player->GetBoneByID(Bone);
	float Dist = Math::Calc3D_Dist(LP_Pos, BonePos);
	if (Dist > 0.001f) {
		float speed = GetBulletSpeed(tar, ammo);
		if (!speed) speed = 250.f;
		float BulletTime = Dist / speed;

		Vector3 vel = Player->GetVelocity();

		Vector3 PredictVel = vel * BulletTime * 0.75f;

		BonePos += PredictVel;
		float gravity = GetGravity(ammo);
		BonePos.y += (4.905f * BulletTime * BulletTime) * gravity;
	}
	return BonePos;
}

void Normalize(float& Yaw, float& Pitch) {
	if (Pitch < -89) Pitch = -89;
	else if (Pitch > 89) Pitch = 89;
	if (Yaw < -360) Yaw += 360;
	else if (Yaw > 360) Yaw -= 360;
}

void GoToTarget(BasePlayer* player)
{
	Vector3 Local = LocalPlayer.BasePlayer->GetBoneByID(head);
	Vector3 PlayerPos = Prediction(Local, player, BoneList(Vars::Global::BoneToAim));

	Vector2 Offset = Math::CalcAngle(Local, PlayerPos) - LocalPlayer.BasePlayer->GetVA();
	Vector2 RecoilAng = LocalPlayer.BasePlayer->GetRA();
	Normalize(Offset.y, Offset.x);

	Vector2 AngleToAim = LocalPlayer.BasePlayer->GetVA() + Offset;

	if (Vars::AimBot::RCS)
	{
		AngleToAim = AngleToAim - RecoilAng;
	}

	Normalize(AngleToAim.y, AngleToAim.x);
	LocalPlayer.BasePlayer->SetVA(AngleToAim);
}

void Aim(BasePlayer* AimEntity)
{
	if (Vars::AimBot::Activate)
	{
		if (Vars::AimBot::RCS)
			LocalPlayer.BasePlayer->SetRA();

		if (AimEntity && !LocalPlayer.BasePlayer->IsMenu()) {
			if (FC(user32, GetAsyncKeyState, Vars::AimBot::aimkey))
				GoToTarget(AimEntity);
		}
	}
}