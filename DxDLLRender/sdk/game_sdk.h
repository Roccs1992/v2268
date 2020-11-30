//game structs
#define g_BN 0x2988DF0 
#define g_BN_Steam 0x2988DF0
#define oPlayerModel 0x4A8 //public PlayerModel playerModel;
#define oPlayerFlags 0x5F8 //	public BasePlayer.PlayerFlags playerFlags;
#define oHeld 0x98  // private EntityRef heldEntity;
#define oPlayerInventory 0x608  //	public PlayerInventory inventory;
#define oPlayerInput 0x4C8  //	public PlayerInput input;
#define oPlayerMovement 0x4D0  // public BaseMovement movement;
#define oSteamID 0x640 //	public ulong userID;
#define oPlayerName 0x650    //	protected string _displayName
#define oNoBlockAiming 0x6B0 //public bool clothingBlocksAiming;
#define oSpeedReduction 0x6B4 //	public float clothingMoveSpeedReduction;
#define oMagazine 0x2A0 
#define oVisible 0x248  // internal bool visible;
#define oNoAimSway 0x6BC     //	public float clothingAccuracyBonus;
#define oLifestate 0x204 //	public BaseCombatEntity.LifeState lifestate;
#define oClientTeam 0x540 // public PlayerTeam clientTeam
#define oActiveUID 0x570  //	private uint clActiveItem;
#define oPlayerHealth 0x20C  //private float _health;
#define oAuto 0x270   // public bool automatic;
#define oDistance 0x278 //private Transform attachmentBoneCache; public float maxDistance; // 0x278
#define oAttackRadius 0x27C //public float attackRadius;
#define oVelocity 0x1FC  // 	private Vector3 newVelocity;
#define oFakeAdmin 0x5F8   //public BasePlayer.PlayerFlags playerFlags;
#define oSuccessFraction 0x340  //public float successFraction;
#define oGroundAngle 0xb4 // private float groundAngle;
#define oGroundAngleNew 0xB8  // private float groundAngleNew;
#define oGravity 0x74  //public float gravityMultiplier;
#define oWaterBoost 0x6B8   //	public float clothingWaterSpeedBonus;
#define oFlyHack  0x13C     // private bool flying;
#define oMultiplierSwimming 0x78  // public float gravityMultiplierSwimming;
#define oTickTime 0x5CC //	private float lastSentTickTime;
#define oFrozen 0x4B0 // public bool Frozen;
#define oKeyFocus 0x94  // private bool hasKeyFocus;
#define oClientTickInterval 0x5C8  // public float clientTickInterval;
//#define oNoClip  0x563900     //public static void noclip(ConsoleSystem.Arg arg) { }
//Recoil
#define oRecoilMinYaw 0x18 //public float recoilYawMin
#define oRecoilMaxYaw 0x1C
#define oRecoilMinPitch 0x20
#define oRecoilMaxPitch 0x24
#define oADSScale 0x30 //public float ADSScale;
#define oMovementPenalty 0x34 //public float movementPenalty;
#define oRecoil 0x2C0  //public RecoilProperties recoil;
//AntiSpread
#define oAimconePenaltyPerShot 0x2D8  // public float aimconePenaltyPerShot;
#define oAimCone 0x2D0  // public float aimCone;
#define oHipAimCone 0x2D4 // public float hipAimCone;
#define oStancePenalty 0x304 // private float stancePenalty;
#define oAimConePenalty 0x308 // private float aimconePenalty;

ULONG64 fpTransform = GetModBase(StrW(L"GameAssembly.dll")) + 0x1A8EB00;  // RVA: 0x1A77CB0 Offset: 0x1A764B0 VA: 0x181A77CB0 //public Vector3 get_position() { }
ULONG64 fpTransform_Steam = GetModBase(StrW(L"GameAssembly.dll")) + 0x1A8EB00;

const unsigned short Crc16Table[256] = {
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

enum BoneList : int
{
	pelvis = 0,
	l_hip = 1,
	l_knee = 2,
	l_foot = 3,
	l_toe = 4,
	l_ankle_scale = 5,
	penis = 6,
	GenitalCensor = 7,
	GenitalCensor_LOD0 = 8,
	Inner_LOD0 = 9,
	GenitalCensor_LOD1 = 11,
	GenitalCensor_LOD2 = 12,
	r_hip = 13,
	r_knee = 14,
	r_foot = 15,
	r_toe = 16,
	r_ankle_scale = 17,
	spine1 = 18,
	spine1_scale = 19,
	spine2 = 20,
	spine3 = 21,
	spine4 = 22,
	l_clavicle = 23,
	l_upperarm = 24,
	l_forearm = 25,
	l_hand = 26,
	l_index1 = 27,
	l_index2 = 28,
	l_index3 = 29,
	l_little1 = 30,
	l_little2 = 31,
	l_little3 = 32,
	l_middle1 = 33,
	l_middle2 = 34,
	l_middle3 = 35,
	l_prop = 36,
	l_ring1 = 37,
	l_ring2 = 38,
	l_ring3 = 39,
	l_thumb1 = 40,
	l_thumb2 = 41,
	l_thumb3 = 42,
	IKtarget_righthand_min = 43,
	IKtarget_righthand_max = 44,
	l_ulna = 45,
	neck = 46,
	head = 47,
	jaw = 48,
	eyeTranform = 49,
	l_eye = 50,
	l_Eyelid = 51,
	r_eye = 52,
	r_Eyelid = 53,
	r_clavicle = 54,
	r_upperarm = 55,
	r_forearm = 56,
	r_hand = 57,
	r_index1 = 58,
	r_index2 = 59,
	r_index3 = 60,
	r_little1 = 61,
	r_little2 = 62,
	r_little3 = 63,
	r_middle2 = 65,
	r_middle3 = 66,
	r_prop = 67,
	r_ring1 = 68,
	r_ring2 = 69,
	r_ring3 = 70,
	r_thumb1 = 71,
	r_thumb2 = 72,
	r_thumb3 = 73,
	IKtarget_lefthand_min = 74,
	IKtarget_lefthand_max = 75,
	r_ulna = 76,
	l_breast = 77,
	r_breast = 78,
	BoobCensor = 79,
	BreastCensor_LOD0 = 80,
	BreastCensor_LOD1 = 83,
	BreastCensor_LOD2 = 84
};

typedef struct _UncStr
{
	char stub[0x10];
	int len;
	wchar_t str[1];
} *pUncStr;

class WeaponData
{

public:
	int LoadedAmmo() {
		DWORD64 Held = safe_read((DWORD64)this + oHeld, DWORD64);
		//printf("Held %lld\n", Held);
		if (Held <= 0) return 0;
		DWORD64 Magazine = safe_read(Held + oMagazine, DWORD64);
		//printf("Magazine %lld\n", Magazine);
		if (Magazine <= 0 || Magazine == 0x3F000000) {
			return 0;
		}
		DWORD64 ammoType = safe_read(Magazine + 0x20, DWORD64); //?
		//printf("ammoType %lld\n", ammoType);
		if (ammoType <= 0 || ammoType == 0x3F000000) {
			return 0;
		}
		int ammo = safe_read(ammoType + 0x18, int);
		//printf("ammo %lld\n", ammo);
		return ammo;
	}


private:
	unsigned short CRC(unsigned char* pcBlock, unsigned short len)
	{
		unsigned short crc = 0xFFFF;
		while (len--)
			crc = (crc << 8) ^ Crc16Table[(crc >> 8) ^ *pcBlock++];
		return crc;
	}

public:
	wchar_t* GetShortName(int* len)
	{
		DWORD64 Info = safe_read(this + 0x20, DWORD64);
		pUncStr Str = ((pUncStr)safe_read(Info + 0x20, DWORD64));
		int leng = safe_read(Str + 0x10, int);
		if (!leng) return nullptr;
		if (len)*len = leng;
		return Str->str;
	}

	USHORT GetNameHash()
	{
		int Len;
		UCHAR* ShortName = (UCHAR*)GetShortName(&Len);
		if (ShortName == nullptr)
			return 0;

		return CRC(ShortName, (Len * 2));
	}

	wchar_t* GetName()
	{
		DWORD64 Info = safe_read(this + 0x20, DWORD64); // private string _targetName;
		DWORD64 DispName = safe_read(Info + 0x28, DWORD64); // public string TargetName;
		pUncStr Str = ((pUncStr)safe_read(DispName + 0x18, DWORD64)); // private string _TargetNamespace;
		if (!Str) return nullptr; return Str->str;
	}
	int GetID() {
		DWORD64 Info = safe_read((DWORD64)this + 0x20, DWORD64);
		int ID = safe_read(Info + 0x18, int);
		return ID;
	}
	Weapon Info() {
		int ID = GetID();
		printf("Active UID %d\n", ID);
		for (Weapon k : info) {
			if (k.id == ID) {
				return k;
			}
		}
		return Weapon{ 0 };
	}
	DWORD64 GetUID()
	{
		DWORD64 uid = safe_read(this + 0x28, DWORD64); // public uint uid
		return uid;
	}

	void NoRecoil()
	{
		if (Vars::Weapon::NoRecoil)
		{
			DWORD64 Held = safe_read(this + oHeld, DWORD64);
			DWORD64 recoil = safe_read(Held + oRecoil, DWORD64); //public RecoilProperties recoil; не обновились оффсеты.
			safe_write(recoil + oRecoilMinYaw,Vars::Weapon::Recoil, float); //public float recoilYawMin; не обновились оффсеты.
			safe_write(recoil + oRecoilMaxYaw, Vars::Weapon::Recoil, float); //public float recoilYawMax; не обновились оффсеты.
			safe_write(recoil + oRecoilMinPitch, Vars::Weapon::Recoil, float); //public float recoilPitchMin; не обновились оффсеты.
			safe_write(recoil + oRecoilMaxPitch, Vars::Weapon::Recoil, float); //public float recoilPitchMax; не обновились оффсеты.
			safe_write(recoil + oADSScale, Vars::Weapon::Recoil, float); //public float ADSScale; не обновились оффсеты.
			safe_write(recoil + oMovementPenalty, Vars::Weapon::Recoil, float); //public float movementPenalty; не обновились оффсеты.
	//Работает
		}
	}

	void AntiSpread()
	{
		if (Vars::Weapon::AntiSpread)
		{
			DWORD64 Held = safe_read(this + oHeld, DWORD64); // EntityRef heldEntity
			safe_write(Held + oStancePenalty, 0.f, float); // private float stancePenalty;
			safe_write(Held + oAimConePenalty, 0.f, float); // private float aimconePenalty;
			safe_write(Held + oAimCone, 0.f, float); // public float aimCone;
			safe_write(Held + oHipAimCone, 0.f, float); // public float hipAimCone;
			safe_write(Held + oAimconePenaltyPerShot, 0.f, float); // public float aimconePenaltyPerShot;
		}
	}

	void Compund() {
		if (Vars::Misc::compound)
		{
			//DWORD64 Held = safe_read(this + oHeld, DWORD64); // EntityRef heldEntity
			safe_write(this + 0x358, 0.f, float); // compound->stringmaxduration   // public float stringHoldDurationMax;
		}
	}
	void SuperBow()
	{
		if (Vars::Misc::SuperBow)
		{
			DWORD64 Held = safe_read(this + oHeld, DWORD64); // EntityRef heldEntity
			safe_write(Held + 0x340, 1, bool); // protected bool attackReady;
			safe_write(Held + 0x344, 1.f, float); // private float arrowBack;
		}
	}
	
	void SetAutomatic()
	{
		if (Vars::Weapon::SetAutomatic)
		{
			DWORD64 Held = safe_read(this + oHeld, DWORD64);
			safe_write(Held + oAuto, 1, bool); // public bool automatic;
		}
	}

	void FatBullet()
	{
		if (Vars::Misc::FatBullet)
		{
			DWORD64 Held = safe_read(this + oHeld, DWORD64); //	private EntityRef heldEntity; // 0x90
			DWORD64 List = safe_read(Held + 0x338, DWORD64); //private List<Projectile> createdProjectiles; // 0x338
			int size = safe_read(List + 0x18, DWORD64);
			List = safe_read(List + 0x10, DWORD64);
			{
				for (int i = 0; i < size; ++i)
				{
					UINT64 Item = safe_read(List + 0x20 + (i * 0x8), UINT64);
					safe_write(Item + 0x2C, 0.7f, float); //public float thickness; // 0x2C??
				}
			}

		}
	}

	void setRemoveFlag(int flag)
	{
		DWORD64 mstate = safe_read(this + 0x588, DWORD64);

		int flags = safe_read(mstate + 0x24, int);
		safe_write(mstate + 0x24, flags &= ~flag, int)
	}

	void SuperEoka()
	{
		DWORD64 Held = safe_read(this + oHeld, DWORD64); // EntityRef heldEntity
		safe_write(Held + oSuccessFraction, 1.f, float); //public float successFraction;
	}


	void LongHand()
	{
		DWORD64 Held = safe_read(this + oHeld, DWORD64); // EntityRef heldEntity
		safe_write(Held + oDistance, 4.5f, float); //private Transform attachmentBoneCache; // 
	}

	void FatHand()
	{
		DWORD64 Held = safe_read(this + oHeld, DWORD64); // EntityRef heldEntity
		safe_write(Held + oAttackRadius, 15.f, float); //public float attackRadius;
	}

	void RunOnHit()
	{
		DWORD64 Held = safe_read(this + oHeld, DWORD64); // EntityRef heldEntity
		safe_write(Held + 0x281, 0, int); //public bool blockSprintOnAttack;
	}

};

class BasePlayer
{
public:
	//*** base entity info ***//
	
	Vector3 GetPosition() {
		return GetBone(GetTransform(Vars::Global::BoneToAim));
	}

	/*Vector3 GetBoneByID(BoneList BoneID) {
		return GetBone(GetTransform(BoneID));
	}*/
	
	float GetHealth()

	{
		return safe_read(this + oPlayerHealth, float); //private float _health;
		//обновил
	}

	float HeliHealtch() {

		return safe_read(this + 0x24, float); //public float health; // 0x24
	}

	void Compund() {

		if (Vars::Misc::compound)
			safe_write(this + 0x358, 0.f, float); // compound->stringmaxduration   // public float stringHoldDurationMax;
	}

	void SetFov()
	{
		auto klass = safe_read(Vars::Storage::gBase + 0x2984A00, DWORD64); //"Name": "ConVar.Graphics_TypeInfo",  //"Signature": "ConVar_Graphics_c*"
		auto static_fields = safe_read(klass + 0xB8, DWORD64);
		safe_write(static_fields + 0x18, Vars::Misc::GraphicsFov, float);
	}

	const wchar_t* GetName()
	{
		pUncStr Str = ((pUncStr)(safe_read(this + oPlayerName, DWORD64))); //	protected string _displayName
		if (!Str) return L""; return Str->str;
		//обновил
	}

	Vector3 GetVelocity()
	{
		DWORD64 PlayerModel = safe_read(this + oPlayerModel, DWORD64); //	public PlayerModel playerModel;
		return safe_read(PlayerModel + oVelocity, Vector3); // 	private Vector3 newVelocity;
		//обновил
	}

	bool IsVisible()
	{
		if (Vars::AimBot::VisibleCheck)
		{
			DWORD64 PlayerModel = safe_read(this + oPlayerModel, DWORD64); //public PlayerModel playerModel;
			return safe_read(PlayerModel + oVisible, bool); // internal bool visible;
		}
		else return true;
		//обновил
	}

	float GetTickTime()
	{
		return safe_read(this + oTickTime, float); //	private float lastSentTickTime;
		//обновил
	}

	bool IsValid(bool LPlayer = false)
	{
		return (((LPlayer || Vars::PlayerEsp::IgnoreSleepers) ? !HasFlags(16) : true) && !IsDead() && (GetHealth() >= 0.8f)); GetTeamSize();
	}

	bool HasFlags(int Flg)
	{
		return (safe_read(this + oPlayerFlags, int) & Flg); //	public BasePlayer.PlayerFlags playerFlags;
		//обновил
	}

	Vector3 GetBoneByID(BoneList BoneID) {
		return GetPosition(GetTransform(BoneID));
	}

	int GetTeamSize()
	{
		DWORD64 ClientTeam = safe_read(this + oClientTeam, DWORD64); // public PlayerTeam clientTeam
		DWORD64 members = safe_read(ClientTeam + 0x30, DWORD64);//    private ListHashSet1<ILOD> members; // 0x28 ���  public List1<PlayerTeam.TeamMember> members; // 0x30
		return safe_read(members + 0x18, DWORD64);
	}

	DWORD64 IsTeamMate(int Id)
	{
		DWORD64 ClientTeam = safe_read(this + oClientTeam, DWORD64);//    public PlayerTeam clientTeam;
		DWORD64 members = safe_read(ClientTeam + 0x30, DWORD64);//    private ListHashSet1<ILOD> members; // 0x28 ���  public List1<PlayerTeam.TeamMember> members; // 0x30
		DWORD64 List = safe_read(members + 0x10, DWORD64);
		DWORD64 player = safe_read(List + 0x20 + (Id * 0x8), DWORD64); //     private BasePlayer player;
		return safe_read(player + 0x20, DWORD64);
	}

	/*void Fly()
	{
		if (Vars::Misc::FlyHack)
		{
			DWORD64 Movement = safe_read(this + oPlayerMovement, DWORD64);
			if (!Movement)
				return;

			safe_write(Movement + oFlyHack, 1, bool); // private bool flying;
		}
	}*/

	void Swim()
	{
		if (Vars::Misc::Swimming)
		{
			DWORD64 ModelState = safe_read(this + 0x578, DWORD64);
			if (!ModelState)
				return;

			safe_write(ModelState + oFlyHack, 1, bool); // private bool flying;
			safe_write(ModelState + oWaterBoost, 1.f, float); //	public float clothingWaterSpeedBonus;
			safe_write(ModelState + oFlyHack, Vars::Misc::SetSwim, float); // private bool flying;
		}
	}

	void WaterBoost()
	{
		safe_write(this + oWaterBoost, 0.15f, float); //	public float clothingWaterSpeedBonus;
		//РАБОТАЕТ
	}

	bool IsDead()
	{
		if (!this) return true;
		return safe_read(this + oLifestate, bool); //	public BaseCombatEntity.LifeState lifestate;
	}


	DWORD64 GetSteamID()
	{
		return safe_read(this + oSteamID, DWORD64);//	public ulong userID;
	}

	//*** localplayer ***//
	bool IsMenu()
	{
		if (!this) return true;
		DWORD64 Input = safe_read(this + oPlayerInput, DWORD64); //	public PlayerInput input;
		return !(safe_read(Input + oKeyFocus, bool));// private bool hasKeyFocus;
	}

	/*void NoClip()
	{
		DWORD64 LocalMe = safe_read(this + 0x28, DWORD64);
		safe_write(LocalMe + 0x7F78D0, true, bool);
	}*/

	void SetVA(const Vector2& VA)
	{
		DWORD64 Input = safe_read(this + oPlayerInput, DWORD64);//	public PlayerInput input;
		safe_write(Input + 0x3C, VA, Vector2); //private Vector3 bodyAngles;
	}

	void SetRA()
	{
		if (Vars::AimBot::RCS) {
			DWORD64 Input = safe_read(this + oPlayerInput, DWORD64);//	public PlayerInput input;
			Vector2 RA = { 0.f, 0.f };
			safe_write(Input + 0x64, RA, Vector2); //public Vector3 recoilAngles;
		}
	}

	Vector2 GetVA()
	{
		DWORD64 Input = safe_read(this + oPlayerInput, DWORD64);//	public PlayerInput input;
		return safe_read(Input + 0x3C, Vector2);//private Vector3 bodyAngles;
	}

	Vector2 GetRA()
	{
		DWORD64 Input = safe_read(this + oPlayerInput, DWORD64);//	public PlayerInput input;
		return safe_read(Input + 0x64, Vector2);//public Vector3 recoilAngles;
	}

	WeaponData* GetWeaponInfo(int Id)
	{
		DWORD64 Inventory = safe_read(this + oPlayerInventory, DWORD64);//	public PlayerInventory inventory;
		DWORD64 Belt = safe_read(Inventory + 0x28, DWORD64);
		DWORD64 ItemList = safe_read(Belt + 0x38, DWORD64);// 	public List`1<Item> itemList;
		DWORD64 Items = safe_read(ItemList + 0x10, DWORD64); //	public List`1<InventoryItem.Amount> items;
		return (WeaponData*)safe_read(Items + 0x20 + (Id * 0x8), DWORD64);
	}

	WeaponData* GetActiveWeapon()
	{
		if (!this)
			return nullptr;

		int ActUID = safe_read(this + oActiveUID, int); //	private uint clActiveItem;
		if (!ActUID) return nullptr;
		WeaponData* ActiveWeapon;
		for (int i = 0; i < 6; i++)
		{
			if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->GetUID())
				return ActiveWeapon;
		}
		return nullptr;
	}

	WeaponData* GetAtiem()
	{
		if (!this)
			return nullptr;

		int ActUID = safe_read(this + oActiveUID, int); //	private uint clActiveItem;
		if (!ActUID) return nullptr;
		WeaponData* ActiveWeapon;
		for (int i = 0; i < 6; i++)
		{
			if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->GetUID())
				return ActiveWeapon;
		}
		return nullptr;
	}

	//*** cheat func ***//

	void AirStuck(bool state) {
		safe_write(this + oFrozen, state, bool);
	}

	void WalkWater()
	{
		DWORD64 Movement = safe_read(this + oPlayerMovement, DWORD64);
		if (!Movement)
			return;

		static bool x130 = safe_read(Movement + oFlyHack, bool);
		static float xB0 = safe_read(Movement + oGroundAngleNew, float);
		static float x74 = safe_read(Movement + oGravity, float);
		static float x78 = safe_read(Movement + oMultiplierSwimming, float);

		bool state = Vars::Misc::WalkOnWater && GetAsyncKeyState(0x58);

		if (Vars::Misc::WalkOnWater)
		{
			safe_write(Movement + oFlyHack, 1, bool); // private bool flying;
			safe_write(Movement + oGroundAngleNew, 0.f, float);// private float groundAngleNew;
			safe_write(Movement + oGravity, 0.f, float);// public float gravityMultiplier;
			safe_write(Movement + oMultiplierSwimming, 1.f, float);// public float gravityMultiplierSwimming;
		}
		else
		{
			safe_write(Movement + oFlyHack, x130, bool); // private bool flying;
			safe_write(Movement + oGroundAngleNew, xB0, float);// private float groundAngleNew;
			safe_write(Movement + oGravity, x74, float);// public float gravityMultiplier;
			safe_write(Movement + oMultiplierSwimming, x78, float);// public float gravityMultiplierSwimming;
		}
	}

	void NoSway()
	{
		if (Vars::Weapon::NoSway)
		{
			safe_write(this + oNoAimSway, 0.f, float);//	public float clothingAccuracyBonus;
			//РАБОТАЕТ
		}
	}

	void SetRemoveFlag(int flag) {
		DWORD64 mstate = safe_read(this + 0x588, DWORD64); //public ModelState modelState;

		int flags = safe_read(mstate + 0x24, int);
		safe_write(mstate + 0x24, flags &= ~flag, int);
	}


	/*void Up()
	{
		safe_write(this + oNoAimSway, 0.f, float);//	public float clothingAccuracyBonus;
		//РАБОТАЕТ
	}*/

	void FakeAdmin(int Val)
	{

		int Flags = safe_read(this + oPlayerFlags, int); //public BasePlayer.PlayerFlags playerFlags;
		safe_write(this + oPlayerFlags, (Flags |= 4), int);
	}

	/*void FakeWorkBench(int Val)
	{
		int Flags = safe_read(this + oPlayerFlags, int); //public BasePlayer.PlayerFlags playerFlags;
		safe_write(this + oPlayerFlags, (Flags |= 4194304), int);
	}*/

	int GetActiveItemUID()
	{
		return safe_read(this + 0x5E0, UINT64);
	}

	void GetBelt()
	{
		safe_read(this + 0x28, DWORD64); // public ItemContainer containerBelt;
	}

	void GetItems()
	{
		DWORD64 ItemList = safe_read(this + 0x28, DWORD64); // public ItemContainer containerBelt;
	}

	void GetItemDefinition()
	{
		DWORD64 Items = safe_read(this + oPlayerMovement, DWORD64); // public BaseMovement movement;
	}

	wchar_t* GettName()
	{
		DWORD64 Info = safe_read(this + 0x20, DWORD64); // private string _targetName;
		DWORD64 DispName = safe_read(Info + 0x28, DWORD64); // public string TargetName;
		pUncStr Str = ((pUncStr)safe_read(DispName + 0x18, DWORD64)); // private string _TargetNamespace;
		if (!Str) return nullptr; return Str->str;
	}

	void GetInventory()
	{
		safe_read(this + oPlayerMovement, DWORD64); // public BaseMovement movement;
	}

	void SpiderMan()
	{
		DWORD64 Movement = safe_read(this + oPlayerMovement, DWORD64); // public BaseMovement movement;
		safe_write(Movement + oGroundAngle, 0.f, float);// private float groundAngle;
		safe_write(Movement + oGroundAngleNew, 0.f, float);// private float groundAngleNew;
		//РАБОТАЕТ
	}

	void NoBlockAiming()
	{
		safe_write(this + oNoBlockAiming, false, bool);//public bool clothingBlocksAiming;
		//РАБОТАЕТ
	}

	void NoHeavyReduct()
	{
		float Reduct = safe_read(this + oSpeedReduction, float);//	public float clothingMoveSpeedReduction;
		if (Reduct == 0.f) { safe_write(this + oSpeedReduction, -0.1f, float); }//	public float clothingMoveSpeedReduction;
		else if ((Reduct > 0.15f) && (Reduct < 0.35f))
		{
			safe_write(this + oSpeedReduction, 0.15f, float);//	public float clothingMoveSpeedReduction;
		}
		else if (Reduct > 0.39f)
		{
			safe_write(this + oSpeedReduction, 0.15f, float);//	public float clothingMoveSpeedReduction;
		}
		//РАБОТАЕТ
	}

	void SetGravity(float val)
	{
		DWORD64 Movement = safe_read(this + oPlayerMovement, DWORD64);// BaseMovement movement
		safe_write(Movement + oGravity, val, float); //public float gravityMultiplier;
		//РАБОТАЕТ
	}

	typedef Vector3(__stdcall* Transform)(UINT64);
private:
	Vector3 GetBone(ULONG_PTR pTransform)
	{
		if (!isSteam) return Vector3();
		if (pTransform < 0xFFF) return Vector3();
		Transform original;
		if (isSteam == -1) original = (Transform)fpTransform;
		else original = (Transform)fpTransform_Steam;
		if (!safe_read(this + 0x50, DWORD64) || safe_read(this + 0x18, bool) || !safe_read(this + oPlayerModel, DWORD64)) return Vector3();

		Vector3 res = original(pTransform);
		//printf("[+] %llX -> %f %f %f\n", pTransform, res.x, res.y, res.z);
		return res;
	}

public:
	Vector3 GetPosition(DWORD64 transform)
	{
		if (!transform) return Vector3{ 0.f, 0.f, 0.f };
		{

			struct Matrix34 { BYTE vec0[16]; BYTE vec1[16]; BYTE vec2[16]; };
			const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
			const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
			const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

			int Index = *(PINT)(transform + 0x40);
			DWORD64 pTransformData = safe_read(transform + 0x38, DWORD64);
			DWORD64 transformData[2];
			safe_memcpy(&transformData, (PVOID)(pTransformData + 0x18), 16);

			size_t sizeMatriciesBuf = 48 * Index + 48;
			size_t sizeIndicesBuf = 4 * Index + 4;

			int pIndicesBuf[100];
			Matrix34 pMatriciesBuf[1000];

			safe_memcpy(pMatriciesBuf, (PVOID)transformData[0], sizeMatriciesBuf);
			safe_memcpy(pIndicesBuf, (PVOID)transformData[1], sizeIndicesBuf);

			__m128 result = *(__m128*)((ULONGLONG)pMatriciesBuf + 0x30 * Index);
			int transformIndex = *(int*)((ULONGLONG)pIndicesBuf + 0x4 * Index);

			while (transformIndex >= 0)
			{
				Matrix34 matrix34 = *(Matrix34*)((ULONGLONG)pMatriciesBuf + 0x30 * transformIndex);
				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

				result = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
							tmp7)), *(__m128*)(&matrix34.vec0));

				transformIndex = *(int*)((ULONGLONG)pIndicesBuf + 0x4 * transformIndex);
			}

			return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
		}
	}
	DWORD64 GetTransform(int bone)
	{
		DWORD64 player_model = safe_read(this + 0x118, DWORD64);// public Model model;_public class BaseEntity : BaseNetworkable, IProvider, ILerpTarget, IPrefabPreProcess // TypeDefIndex: 8714
		DWORD64 boneTransforms = safe_read(player_model + 0x48, DWORD64);// public Transform[] boneTransforms;
		DWORD64 BoneValue = safe_read((boneTransforms + (0x20 + (bone * 0x8))), DWORD64);
		return safe_read(BoneValue + 0x10, DWORD64);
	}

};

//Base Player
//Base Player
class LPlayerBase
{
public:
	BasePlayer* BasePlayer = nullptr;
	Matrix4x4* pViewMatrix = nullptr;
	bool WorldToScreen(const Vector3& EntityPos, Vector2& ScreenPos)
	{
		if (!pViewMatrix) return false;
		Vector3 TransVec = Vector3(pViewMatrix->_14, pViewMatrix->_24, pViewMatrix->_34);
		Vector3 RightVec = Vector3(pViewMatrix->_11, pViewMatrix->_21, pViewMatrix->_31);
		Vector3 UpVec = Vector3(pViewMatrix->_12, pViewMatrix->_22, pViewMatrix->_32);
		float w = Math::Dot(TransVec, EntityPos) + pViewMatrix->_44;
		if (w < 0.098f) return false;
		float y = Math::Dot(UpVec, EntityPos) + pViewMatrix->_42;
		float x = Math::Dot(RightVec, EntityPos) + pViewMatrix->_41;
		ScreenPos = Vector2((Vars::Global::ScreenWidth / 2) * (1.f + x / w), (Vars::Global::ScreenHigh / 2) * (1.f - y / w));
		return true;
	}
};

DECLSPEC_NOINLINE void Flex() {
	FC(kernel32, Sleep, 0);
}

LPlayerBase LocalPlayer;