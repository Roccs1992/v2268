namespace Vars
{
	HANDLE handle;

	namespace Global
	{
		ULONG_PTR gameObjectManager;
		ULONG_PTR gameObjectOffset = 0x14C1790;
		INT16 tag;
		int closestPlayer;
		BYTE state;
		Vector3 position;
		Vector2 MenuPos = { 30.0f, 30.0f };
		Vector2 ShowPlayerBox = { 150, 150 };
		RECT rGameWindow;
		bool DrawCircle = false;
		int ScreenHigh = 0;
		int ScreenWidth = 0;
		int GameWindow;
		HWND hWindow = nullptr;
		bool MenuVisible = false;
		WNDPROC oWindproc = nullptr;

		Vector3 LastPos = { 0.f, 0.f, 0.f };
		ULONGLONG LastTime = 0;
		DWORD64 LastSteamID = 0;
		const wchar_t* LastName;
		Vector3 PlayerVeloc = { 0.f, 0.f, 0.f };

		float BoneToAim = 0.5;

		float BulletSpeed = 250.f;
		DWORD64 PresentDelta = 0;
		float CorX = 0.f;
		float CorY = 0.f;

		int CurPage = 0;
	}

	namespace Other
	{
		bool bUnloading = false;
	}

	namespace Storage
	{
		__declspec(selectany) uintptr_t gBase = (DWORD64)GetModuleHandleA("GameAssembly.dll");
	}

	namespace AimBot
	{
		bool SilentAim = false;
		bool pSilent = false;
		bool DrawSilentFOV = false;
		float SilentFov = 600.f;
		bool SilentBody = false;
		bool canSprint = false;
		bool SilentTarget = false;
		bool ThroughWall = false;
		bool ThroughTerrain = false;
		bool Activate = false;
		bool BodyAim = false;
		bool CanAttack = false;
		bool OmniSprint = false;
		bool RCS = false;
		bool Distance = false;
		bool IgnoreTeam = false;
		bool IgnoreNpc = false;
		bool BoneToAim = false;
		bool VisibleCheck = false;
		bool DrawFov = false;
		bool Crosshair = false;
		float Range = 180.f;
		bool FillFov = false;
		float Fov = 90.f;
		int aimkey = 0;
		static int keystatus = 0;
		int jumpkey = 0;
		static int jumpkeystatus = 0;
		bool JumpShot = false;
	}

	namespace Weapon
	{
		bool Weapon1 = false;
		bool NoRecoil = false;
		float Recoil = 1.f;
		bool AntiSpread = false;
		bool SetAutomatic = false;
		bool NoSway = false;
		bool SuperEoka = false;
	}

	namespace PlayerEsp
	{
		bool ShowPlayerBox = false;
		bool SkeletonPlayer = false;
		bool ShowPlayerHealth = false;
		bool ShowPlayerWeapon = false;
		bool ShowPlayerDist = false;
		bool IgnoreSleepers = false;
		bool PlayerWounded = false;
		bool IgnoreNpc = false;
		bool SnapLines = false;
		bool Skeleton = false;
		bool ShowPlayerName = false;
		bool FillBox = false;

	}


	namespace Misc
	{
		bool Misk = false;
		bool HookFatBullet = false;
		bool FatBullet = false;
		bool SpiderMan = false;
		bool SuperBow = true;
		bool NoClip = false;
		bool WalkOnWater = false;
		bool FakeAdmin = false;
		bool CustomTime = false;
		float Time = 1.0f;
		bool HighJump = false;
		bool AlwaysDay = false;
		float JumpValue = 1.2f;
		bool WaterBoost = false;
		bool NoHeavyReduct = false;
		bool NoBlockAiming = false;
		float GraphicsFov = 100.f;
		bool LongHand = false;
		float SetSwim = 0.f;
		bool Swimming = false;
		bool CustomFov = false;
		bool AirStuck = false;
		bool ShotGunTochka = false;
		bool SpeedHack = false;
		float SpeedValue = 5;
		bool IsThirdPerson = false;
		bool FakeLag = false;
		bool AntiAim = false;
		bool compound = false;
		bool SilentWalk = false;
		float DistanceAnimals = 100.f;
	}

	namespace Distance 
	{

		float Animals = 100.f;

	}

	namespace Animals
	{
		bool Wolf = false;
		bool Deer = false;
		bool Horse = false;
		bool Chicken = false;
		bool Boar = false;
		bool Bears = false;
	}

	namespace technique
	{
		bool cars = false;
		bool Rowboat = false;
		bool RHIB = false;
		bool Kayak = false;
		//bool Tank = false;
	}

	namespace Visuals
	{
		bool Resource = false;
		bool Stone = false;
		bool Sulphur = false;
		bool Metal = false;
		bool PickUp_Metal = false;
		bool PickUp_Stone = false;
		bool PickUp_Sulfur = false;
		bool PickUp_Wood = false;
		bool Stash = false;
		bool Hemp = false;
		bool Minicopter = false;
		bool PatrolHeli = false;
		bool Guntrap = false;
		bool Corpse = false;
		bool Backpack = false;
		bool Mushrooms = false;
		bool bagloot = false;
		bool Drop = false;
		bool Airdrop = false;
		bool ScrapHeli = false;
		bool Crate = false;
		bool dropbox = false;
		bool Turret = false;
		bool Barrel = false;
		bool Cargo = false;
		//    Building Color
		bool Furnace = false;
		bool campfire = false;
		bool Cupboard = false;
		bool LWB = false;
		//   Food
		bool Pumpkin = false;
		bool Potato = false;
		bool Corn = false;
		bool Berry = false;
	}

	namespace Color {

		float SleepersColor[] = { 255.f,255.f,255.f, 1.f };
		float WoundedColor[] = { 255.f, 255.f, 255.f, 1.f };
		float SnapColors[] = { 255.f,255.f,255.f,1.f };
		float Fov[] = { 0.f, 0.f, 255.f, 255.f };
		float SkeletonColors[] = { 255.f, 255.f, 255.f, 1.f };
		float BoxColors[] = { 255.f, 255.f, 255.f, 1.f };
		float BoxFilledColor[] = { 0.f, 0.f, 0.f, 0.3f };
		float WeaponColors[] = { 255.f, 255.f, 255.f, 1.f };
		float CrosshairColors[] = { 255.f, 255.f, 255.f, 1.f };
		float HpBar[] = { 0.f, 255.f, 0.f, 1.f };
		float EmptyHpBar[] = { 255.f, 0.f, 0.f, 1.f  };
		float CorpseColor[] = { 0.f, 0.f, 0.f, 1.f };
		float PlayerRenderStringColor[] = { 1.f, 1.f, 1.f, 1.f };
		float BotBoxColors[] = { 0.f, 0.f, 0.f, 1.f };
		float FillFovColors[] = { 0.f, 0.f, 0.f, 0.3f };
		//    Resource Color
		float StoneColor[] = { 255.f,255.f,255.f, 1.f };
		float SulphurColor[] = { 255.f,255.f,255.f, 1.f };
		float MetalColor[] = { 255.f,255.f,255.f, 1.f };
		float PickUp_StoneColor[] = { 255.f,255.f,255.f, 1.f };
		float PickUp_SulfurColor[] = { 255.f,255.f,255.f, 1.f };
		float PickUp_MetalColor[] = { 255.f,255.f,255.f, 1.f };
		float PickUp_WoodColor[] = { 255.f,255.f,255.f, 1.f };
		float HempColor[] = { 255.f,255.f,255.f, 1.f };
		float MushroomsColor[] = { 255.f,255.f,255.f, 1.f };
		//    Building Color
		float CupboardColor[] = { 0.5f, 0.35f, 0.05f, 1.f };
		float LWBColor[] = { 0.500f, 0.200f, 0.05f, 1.f };
		float WSBColor[] = { 0.450f, 0.125f, 0.155f, 1.f };
		float CampFireCol[] = { 255.f,255.f,255.f, 1.f };
		float FurnaceCol[] = { 255.f,255.f,255.f, 1.f };
		//    Misc Esp Color
		float StashColor[] = { 255.f,255.f,255.f, 1.f };
		float MinicopterColor[] = { 255.f,255.f,255.f, 1.f };
		float TurretColor[] = { 255.f,255.f,255.f, 1.f };
		float GuntrapColor[] = { 255.f,255.f,255.f, 1.f };
		float BackpackColor[] = { 255.f,255.f,255.f, 1.f };
		float AirdropColor[] = { 255.f,255.f,255.f, 1.f };
		float SilentFovColor[] = { 0.f, 0.f, 0.f };
		float SilentColor[] = { 255, 255, 0 };
		float BagLootColor[] = { 255.f,255.f,255.f, 1.f };
		float BarrelCol[] = { 255.f,255.f,255.f, 1.f };
		float CargoCol[] = { 255.f,255.f,255.f, 1.f };
		float PatrolHeliCol[] = { 255.f,255.f,255.f, 1.f };
		//    Animals
		float WolfColor[] = { 255.f,255.f,255.f, 1.f };
		float DeerColor[] = { 255.f,255.f,255.f, 1.f };
		float HorseColor[] = { 255.f,255.f,255.f, 1.f };
		float ChickenColor[] = { 255.f,255.f,255.f, 1.f };
		float BoarColor[] = { 255.f,255.f,255.f, 1.f };
		float BearsColor[] = { 255.f,255.f,255.f, 1.f };
		//    Tech
		float CarsCol[] = { 255.f,255.f,255.f, 1.f };
		float RowboatCol[] = { 255.f,255.f,255.f, 1.f };
		float RHIBCol[] = { 255.f,255.f,255.f, 1.f };
		float KayakCol[] = { 255.f,255.f,255.f, 1.f };
		//   Food
		float PumpkinColor[] = { 255.f,255.f,255.f, 1.f };
		float PotatoColor[] = { 255.f,255.f,255.f, 1.f };
		float CornColor[] = { 255.f,255.f,255.f, 1.f };
		float BerryColor[] = { 255.f,255.f,255.f, 1.f };
	}
	namespace Config {
		static char configname[128] = "My";
		bool LoadConfig = false;
		bool SaveConfig = false;
		bool InitConfig = false;
	}

}