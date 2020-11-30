#include <string>
#include <vector>
#include <Windows.h>
#include"vars.h"


template <typename T>
class VarType
{
public:
	VarType(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class CConfig
{
public:
	CConfig()
	{
		ints = std::vector<VarType<int>*>();
		bools = std::vector<VarType<bool>*>();
		floats = std::vector<VarType<float>*>();
	}

	void Initialize()
	{ 
		szPath = "C:\\config\\";
		Setup();
	}

	void Save()
	{
		std::string file;

		file = szPath + Vars::Config::configname;

		CreateDirectoryA(szPath.c_str(), NULL);

		for (VarType<int>* pValue : ints)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<float>* pValue : floats)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<bool>* pValue : bools)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), *pValue->value ? "1" : "0", file.c_str());
	}

	void Load()
	{
		std::string file;

		file = szPath + Vars::Config::configname;

		CreateDirectoryA(szPath.c_str(), NULL);

		char value_l[32] = { '\0' };

		for (VarType<int>* pValue : ints)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = atoi(value_l);
		}

		for (VarType<float>* pValue : floats)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = (float)atof(value_l);
		}

		for (VarType<bool>* pValue : bools)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = !strcmp(value_l, "1");
		}
	}

private:
	void SetupValue(int &value, int def, std::string category, std::string name)
	{
		value = def;
		ints.push_back(new VarType<int>(category, name, &value));
	}

	void SetupValue(bool &value, bool def, std::string category, std::string name)
	{
		value = def;
		bools.push_back(new VarType<bool>(category, name, &value));
	}

	void SetupValue(float &value, float def, std::string category, std::string name)
	{
		value = def;
		floats.push_back(new VarType<float>(category, name, &value));
	}

	void Setup()
	{
		//AimBot
		SetupValue(Vars::AimBot::Activate, false, "AimBot", "Activate");
		SetupValue(Vars::AimBot::BoneToAim, false, "AimBot", "BodyAim");
		SetupValue(Vars::AimBot::VisibleCheck, false, "AimBot", "Visible Check");
		SetupValue(Vars::AimBot::IgnoreNpc, false, "AimBot", "Ignore Npc");
		SetupValue(Vars::AimBot::IgnoreTeam, false, "AimBot", "IgnoreTeam");
		SetupValue(Vars::AimBot::DrawFov, false, "AimBot", "DrawFov");
		SetupValue(Vars::AimBot::Distance, false, "AimBot", "Distance");
		SetupValue(Vars::AimBot::Range, 180.f, "AimBot", "Range");
		SetupValue(Vars::AimBot::Fov, 10.f, "AimBot", "Fov");
		SetupValue(Vars::AimBot::SilentAim, false, "AimBot", "Silent");
		SetupValue(Vars::AimBot::FillFov, false, "AimBot", "FillFov");
		SetupValue(Vars::AimBot::Crosshair, false, "AimBot", "Crosshair");
		// weapons
		SetupValue(Vars::Weapon::NoRecoil, false, "Weapons", "NoRecoil");
		SetupValue(Vars::Weapon::AntiSpread, false, "Misc", "AntiSpread");
		SetupValue(Vars::Weapon::SetAutomatic, false, "Misc", "SetAutomatic");
		SetupValue(Vars::Weapon::NoSway, false, "Misc", "NoSway");
		SetupValue(Vars::Weapon::SuperEoka, false, "Misc", "SuperEoka");
		//PlayerEsp
		SetupValue(Vars::PlayerEsp::ShowPlayerBox, false, "Visuals", "ShowPlayerBox");
		SetupValue(Vars::PlayerEsp::ShowPlayerName, false, "Visuals", "ShowPlayerName");
		SetupValue(Vars::PlayerEsp::ShowPlayerHealth, false, "Visuals", "ShowPlayerHealth");
		SetupValue(Vars::PlayerEsp::ShowPlayerWeapon, false, "Visuals", "ShowPlayerWeapon");
		SetupValue(Vars::PlayerEsp::ShowPlayerDist, false, "Visuals", "ShowPlayerDist");
		SetupValue(Vars::PlayerEsp::PlayerWounded, false, "Visuals", "PlayerWounded");
		SetupValue(Vars::PlayerEsp::FillBox, false, "Visuals", "FillBox");
		SetupValue(Vars::PlayerEsp::SnapLines, false, "Visuals", "SnapLines");
		SetupValue(Vars::PlayerEsp::Skeleton, false, "Visuals", "Skeleton");
		SetupValue(Vars::PlayerEsp::IgnoreSleepers, false, "Visuals", "IgnoreSleepers");
		SetupValue(Vars::PlayerEsp::IgnoreNpc, false, "Visuals", "IgnoreNpc");
		//visuals
		SetupValue(Vars::Visuals::Stone, false, "Visuals", "Stone");
		SetupValue(Vars::Visuals::Metal, false, "Visuals", "Metal");
		SetupValue(Vars::Visuals::Sulphur, false, "Visuals", "Sulphur");
		SetupValue(Vars::Visuals::Hemp, false, "Visuals", "Hemp");
		SetupValue(Vars::Visuals::PickUp_Metal, false, "Visuals", "PickUp_Metal");
		SetupValue(Vars::Visuals::PickUp_Stone, false, "Visuals", "PickUp_Stone");
		SetupValue(Vars::Visuals::PickUp_Sulfur, false, "Visuals", "PickUp_Sulfur");
		SetupValue(Vars::Visuals::PickUp_Wood, false, "Visuals", "PickUp_Wood");
		SetupValue(Vars::Visuals::Turret, false, "Visuals", "Turret");
		SetupValue(Vars::Visuals::Stash, false, "Visuals", "Stash");
		SetupValue(Vars::Visuals::Minicopter, false, "Visuals", "Minicopter");
		SetupValue(Vars::Visuals::PatrolHeli, false, "Visuals", "PatrolHeli");
		SetupValue(Vars::Visuals::Guntrap, false, "Visuals", "Guntrap"); 
		SetupValue(Vars::Visuals::Backpack, false, "Visuals", "Backpack");
		SetupValue(Vars::Visuals::Airdrop, false, "Visuals", "Airdrop");	
		SetupValue(Vars::Visuals::Corpse, false, "Visuals", "Corpse");
		SetupValue(Vars::Visuals::bagloot, false, "Visuals", "bagloot");
		SetupValue(Vars::Visuals::Cupboard, false, "Visuals", "Cupboard");
		SetupValue(Vars::Visuals::LWB, false, "Visuals", "LWB");
		SetupValue(Vars::Visuals::Barrel, false, "Visuals", "Barrel");
		SetupValue(Vars::Visuals::Cargo, false, "Visuals", "Cargo");
		SetupValue(Vars::Visuals::Furnace, false, "Visuals", "Furnace");
		//Food
		SetupValue(Vars::Visuals::Pumpkin, false, "Visuals", "Pumpkin");
		SetupValue(Vars::Visuals::Potato, false, "Visuals", "Potato");
		SetupValue(Vars::Visuals::Corn, false, "Visuals", "Corn");
		SetupValue(Vars::Visuals::Berry, false, "Visuals", "Berry");
		SetupValue(Vars::Visuals::Mushrooms, false, "Visuals", "Mushrooms");
		//technique
		SetupValue(Vars::technique::cars, false, "technique", "cars");
		SetupValue(Vars::technique::Rowboat, false, "technique", "Rowboat");
		SetupValue(Vars::technique::RHIB, false, "technique", "RHIB");
		SetupValue(Vars::technique::Kayak, false, "technique", "Kayak");
		//Animals
		SetupValue(Vars::Animals::Wolf, false, "Animals", "Wolf");
		SetupValue(Vars::Animals::Horse, false, "Animals", "Horse");
		SetupValue(Vars::Animals::Chicken, false, "Animals", "Chicken");
		SetupValue(Vars::Animals::Boar, false, "Animals", "Boar");
		SetupValue(Vars::Animals::Bears, false, "Animals", "Bears");
		SetupValue(Vars::Animals::Deer, false, "Animals", "Deer");
		//misc
		SetupValue(Vars::Misc::SpiderMan, false, "Misc", "SpiderMan");
		SetupValue(Vars::Misc::FakeAdmin, false, "Misc", "FakeAdmin");
		SetupValue(Vars::Misc::HighJump, false, "Misc", "HighJump");
		SetupValue(Vars::Misc::JumpValue, 1.2f, "Misc", "Jump Multipler");
		SetupValue(Vars::Misc::WaterBoost, false, "Misc", "WaterBoost");
		SetupValue(Vars::Misc::NoHeavyReduct, false, "Misc", "Fast Run");
		SetupValue(Vars::Misc::CustomTime, false, "Misc", "CustomTime");
		SetupValue(Vars::Misc::Time, 0.f, "Misc", "Time");
		SetupValue(Vars::Misc::WalkOnWater, false, "Misc", "WalkOnWater");
		SetupValue(Vars::Misc::NoBlockAiming, false, "Misc", "NoBlockAiming");
		SetupValue(Vars::Misc::AlwaysDay, false, "Visuals", "AlwaysDay");
		SetupValue(Vars::Misc::FatBullet, false, "Misc", "FatBullet");
		//colorpicker
#pragma region LWBColor
		SetupValue(Vars::Color::LWBColor[0], 255.f, "Color", "LWBColorR");
		SetupValue(Vars::Color::LWBColor[1], 255.f, "Color", "LWBColorG");
		SetupValue(Vars::Color::LWBColor[2], 255.f, "Color", "LWBColorB");
#pragma endregion
#pragma region Box
		SetupValue(Vars::Color::BoxColors[0], 255.f, "Color", "BoxR");
		SetupValue(Vars::Color::BoxColors[1], 255.f, "Color", "BoxG");
		SetupValue(Vars::Color::BoxColors[2], 255.f, "Color", "BoxB");
#pragma endregion
#pragma region Skeleton
		SetupValue(Vars::Color::SkeletonColors[0], 255.f, "Color", "SkeletonR");
		SetupValue(Vars::Color::SkeletonColors[1], 255.f, "Color", "SkeletonG");
		SetupValue(Vars::Color::SkeletonColors[2], 255.f, "Color", "SkeletonB");
#pragma endregion
#pragma region BarrelCol
		SetupValue(Vars::Color::BarrelCol[0], 255.f, "Color", "BarrelColR");
		SetupValue(Vars::Color::BarrelCol[1], 255.f, "Color", "BarrelColG");
		SetupValue(Vars::Color::BarrelCol[2], 255.f, "Color", "BarrelColB");
#pragma endregion
#pragma region Fov
		SetupValue(Vars::Color::Fov[0], 255.f, "Color", "DistR");
		SetupValue(Vars::Color::Fov[1], 255.f, "Color", "DistG");
		SetupValue(Vars::Color::Fov[2], 255.f, "Color", "DistB");
#pragma endregion
#pragma region Weapon
		SetupValue(Vars::Color::WeaponColors[0], 255.f, "Color", "WeaponR");
		SetupValue(Vars::Color::WeaponColors[1], 255.f, "Color", "WeaponG");
		SetupValue(Vars::Color::WeaponColors[2], 255.f, "Color", "WeaponB");
#pragma endregion
#pragma region Crosshair
		SetupValue(Vars::Color::CrosshairColors[0], 255.f, "Color", "CrossR");
		SetupValue(Vars::Color::CrosshairColors[1], 255.f, "Color", "CrossG");
		SetupValue(Vars::Color::CrosshairColors[2], 255.f, "Color", "CrossB");
#pragma endregion
#pragma region SleepersColor
		SetupValue(Vars::Color::SleepersColor[0], 255.f, "Color", "SleepersColorR");
		SetupValue(Vars::Color::SleepersColor[1], 255.f, "Color", "SleepersColorG");
		SetupValue(Vars::Color::SleepersColor[2], 255.f, "Color", "SleepersColorB");
#pragma endregion
#pragma region PlayerWoundedColor
		SetupValue(Vars::Color::WoundedColor[0], 255.f, "Color", "WoundedColorR");
		SetupValue(Vars::Color::WoundedColor[1], 255.f, "Color", "WoundedColorG");
		SetupValue(Vars::Color::WoundedColor[2], 255.f, "Color", "WoundedColorB");
#pragma endregion
#pragma region HpBar
		SetupValue(Vars::Color::HpBar[0], 255.f, "Color", "HpBarR");
		SetupValue(Vars::Color::HpBar[1], 255.f, "Color", "HpBarG");
		SetupValue(Vars::Color::HpBar[2], 255.f, "Color", "HpBarB");
#pragma endregion
#pragma region SnapLines
		SetupValue(Vars::Color::SnapColors[0], 255.f, "Color", "SnapLinesR");
		SetupValue(Vars::Color::SnapColors[1], 255.f, "Color", "SnapLinesG");
		SetupValue(Vars::Color::SnapColors[2], 255.f, "Color", "SnapLinesB");
#pragma endregion
#pragma region EmptyHpBar
		SetupValue(Vars::Color::EmptyHpBar[0], 255.f, "Color", "EmptyHpBarR");
		SetupValue(Vars::Color::EmptyHpBar[1], 255.f, "Color", "EmptyHpBarG");
		SetupValue(Vars::Color::EmptyHpBar[2], 255.f, "Color", "EmptyHpBarB");
#pragma endregion
#pragma region PlayerRenderStringColor
		SetupValue(Vars::Color::PlayerRenderStringColor[0], 255.f, "Color", "PlayerRenderStringColorR");
		SetupValue(Vars::Color::PlayerRenderStringColor[1], 255.f, "Color", "PlayerRenderStringColorG");
		SetupValue(Vars::Color::PlayerRenderStringColor[2], 255.f, "Color", "PlayerRenderStringColorB");
#pragma endregion
#pragma region StoneColor
		SetupValue(Vars::Color::StoneColor[0], 255.f, "Color", "StoneColorR");
		SetupValue(Vars::Color::StoneColor[1], 255.f, "Color", "StoneColorG");
		SetupValue(Vars::Color::StoneColor[2], 255.f, "Color", "StoneColorB");
#pragma endregion
#pragma region CorpseColor
		SetupValue(Vars::Color::CorpseColor[0], 255.f, "Color", "CorpseColorR");
		SetupValue(Vars::Color::CorpseColor[1], 255.f, "Color", "CorpseColorG");
		SetupValue(Vars::Color::CorpseColor[2], 255.f, "Color", "CorpseColorB");
#pragma endregion
#pragma region BotBoxColors
		SetupValue(Vars::Color::BotBoxColors[0], 255.f, "Color", "BotBoxColorsR");
		SetupValue(Vars::Color::BotBoxColors[1], 255.f, "Color", "BotBoxColorsG");
		SetupValue(Vars::Color::BotBoxColors[2], 255.f, "Color", "BotBoxColorsB");
#pragma endregion
#pragma region SulphurColor
		SetupValue(Vars::Color::SulphurColor[0], 255.f, "Color", "SulphurColorR");
		SetupValue(Vars::Color::SulphurColor[1], 255.f, "Color", "SulphurColorG");
		SetupValue(Vars::Color::SulphurColor[2], 255.f, "Color", "SulphurColorB");
#pragma endregion
#pragma region MetalColor
		SetupValue(Vars::Color::MetalColor[0], 255.f, "Color", "MetalColorR");
		SetupValue(Vars::Color::MetalColor[1], 255.f, "Color", "MetalColorG");
		SetupValue(Vars::Color::MetalColor[2], 255.f, "Color", "MetalColorB");
#pragma endregion
#pragma region PickUp_StoneColor
		SetupValue(Vars::Color::PickUp_StoneColor[0], 255.f, "Color", "PickUp_StoneColorR");
		SetupValue(Vars::Color::PickUp_StoneColor[1], 255.f, "Color", "PickUp_StoneColorG");
		SetupValue(Vars::Color::PickUp_StoneColor[2], 255.f, "Color", "PickUp_StoneColorB");
#pragma endregion
#pragma region PickUp_SulfurColor
		SetupValue(Vars::Color::PickUp_SulfurColor[0], 255.f, "Color", "PickUp_SulfurColorR");
		SetupValue(Vars::Color::PickUp_SulfurColor[1], 255.f, "Color", "PickUp_SulfurColorG");
		SetupValue(Vars::Color::PickUp_SulfurColor[2], 255.f, "Color", "PickUp_SulfurColorB");
#pragma endregion
#pragma region PickUp_MetalColor
		SetupValue(Vars::Color::PickUp_MetalColor[0], 255.f, "Color", "PickUp_MetalColorR");
		SetupValue(Vars::Color::PickUp_MetalColor[1], 255.f, "Color", "PickUp_MetalColorG");
		SetupValue(Vars::Color::PickUp_MetalColor[2], 255.f, "Color", "PickUp_MetalColorB");
#pragma endregion
#pragma region HempColor
		SetupValue(Vars::Color::HempColor[0], 255.f, "Color", "HempColorR");
		SetupValue(Vars::Color::HempColor[1], 255.f, "Color", "HempColorG");
		SetupValue(Vars::Color::HempColor[2], 255.f, "Color", "HempColorB");
#pragma endregion
#pragma region MushroomsColor
		SetupValue(Vars::Color::MushroomsColor[0], 255.f, "Color", "MushroomsColorR");
		SetupValue(Vars::Color::MushroomsColor[1], 255.f, "Color", "MushroomsColorG");
		SetupValue(Vars::Color::MushroomsColor[2], 255.f, "Color", "MushroomsColorB");
#pragma endregion
#pragma region PickUp_WoodColor
		SetupValue(Vars::Color::PickUp_WoodColor[0], 255.f, "Color", "PickUp_WoodColorR");
		SetupValue(Vars::Color::PickUp_WoodColor[1], 255.f, "Color", "PickUp_WoodColorG");
		SetupValue(Vars::Color::PickUp_WoodColor[2], 255.f, "Color", "PickUp_WoodColorB");
#pragma endregion
#pragma region StashColor
		SetupValue(Vars::Color::StashColor[0], 255.f, "Color", "StashColorR");
		SetupValue(Vars::Color::StashColor[1], 255.f, "Color", "StashColorG");
		SetupValue(Vars::Color::StashColor[2], 255.f, "Color", "StashColorB");
#pragma endregion
#pragma region MinicopterColor
		SetupValue(Vars::Color::MinicopterColor[0], 255.f, "Color", "MinicopterColorR");
		SetupValue(Vars::Color::MinicopterColor[1], 255.f, "Color", "MinicopterColorG");
		SetupValue(Vars::Color::MinicopterColor[2], 255.f, "Color", "MinicopterColorB");
#pragma endregion
#pragma region PatrolHeliCol
		SetupValue(Vars::Color::PatrolHeliCol[0], 255.f, "Color", "PatrolHeliColR");
		SetupValue(Vars::Color::PatrolHeliCol[1], 255.f, "Color", "PatrolHeliColG");
		SetupValue(Vars::Color::PatrolHeliCol[2], 255.f, "Color", "PatrolHeliColB");
#pragma endregion
#pragma region TurretColor
		SetupValue(Vars::Color::TurretColor[0], 255.f, "Color", "TurretColorR");
		SetupValue(Vars::Color::TurretColor[1], 255.f, "Color", "TurretColorG");
		SetupValue(Vars::Color::TurretColor[2], 255.f, "Color", "TurretColorB");
#pragma endregion
#pragma region GuntrapColor
		SetupValue(Vars::Color::GuntrapColor[0], 255.f, "Color", "GuntrapColorR");
		SetupValue(Vars::Color::GuntrapColor[1], 255.f, "Color", "GuntrapColorG");
		SetupValue(Vars::Color::GuntrapColor[2], 255.f, "Color", "GuntrapColorB");
#pragma endregion
#pragma region BackpackColor
		SetupValue(Vars::Color::BackpackColor[0], 255.f, "Color", "BackpackColorR");
		SetupValue(Vars::Color::BackpackColor[1], 255.f, "Color", "BackpackColorG");
		SetupValue(Vars::Color::BackpackColor[2], 255.f, "Color", "BackpackColorB");
#pragma endregion
#pragma region AirdropColor
		SetupValue(Vars::Color::AirdropColor[0], 255.f, "Color", "AirdropColorR");
		SetupValue(Vars::Color::AirdropColor[1], 255.f, "Color", "AirdropColorG");
		SetupValue(Vars::Color::AirdropColor[2], 255.f, "Color", "AirdropColorB");
#pragma endregion
#pragma region BagLootColor
		SetupValue(Vars::Color::BagLootColor[0], 255.f, "Color", "BagLootColorR");
		SetupValue(Vars::Color::BagLootColor[1], 255.f, "Color", "BagLootColorG");
		SetupValue(Vars::Color::BagLootColor[2], 255.f, "Color", "BagLootColorB");
#pragma endregion
#pragma region CupboardColor
		SetupValue(Vars::Color::CupboardColor[0], 255.f, "Color", "CupboardColorR");
		SetupValue(Vars::Color::CupboardColor[1], 255.f, "Color", "CupboardColorG");
		SetupValue(Vars::Color::CupboardColor[2], 255.f, "Color", "CupboardColorB");
#pragma endregion
#pragma region BoxFilledColor
		SetupValue(Vars::Color::BoxFilledColor[0], 255.f, "Color", "BoxFilledColorR");
		SetupValue(Vars::Color::BoxFilledColor[1], 255.f, "Color", "BoxFilledColorG");
		SetupValue(Vars::Color::BoxFilledColor[2], 255.f, "Color", "BoxFilledColorB");
#pragma endregion
#pragma region CargoCol
		SetupValue(Vars::Color::CargoCol[0], 255.f, "Color", "CargoColR");
		SetupValue(Vars::Color::CargoCol[1], 255.f, "Color", "CargoColG");
		SetupValue(Vars::Color::CargoCol[2], 255.f, "Color", "CargoColB");
#pragma endregion
		//Tech
#pragma region CarsCol
		SetupValue(Vars::Color::CarsCol[0], 255.f, "Color", "CarsColR");
		SetupValue(Vars::Color::CarsCol[1], 255.f, "Color", "CarsColG");
		SetupValue(Vars::Color::CarsCol[2], 255.f, "Color", "CarsColB");
#pragma endregion
#pragma region RowboatCol
		SetupValue(Vars::Color::RowboatCol[0], 255.f, "Color", "RowboatColR");
		SetupValue(Vars::Color::RowboatCol[1], 255.f, "Color", "RowboatColG");
		SetupValue(Vars::Color::RowboatCol[2], 255.f, "Color", "RowboatColB");
#pragma endregion
#pragma region Car4Col
		SetupValue(Vars::Color::RHIBCol[0], 255.f, "Color", "RHIBColR");
		SetupValue(Vars::Color::RHIBCol[1], 255.f, "Color", "RHIBColG");
		SetupValue(Vars::Color::RHIBCol[2], 255.f, "Color", "RHIBColB");
#pragma endregion
#pragma region Car4Col
		SetupValue(Vars::Color::KayakCol[0], 255.f, "Color", "KayakColR");
		SetupValue(Vars::Color::KayakCol[1], 255.f, "Color", "KayakColG");
		SetupValue(Vars::Color::KayakCol[2], 255.f, "Color", "KayakColB");
#pragma endregion
		// Animals
#pragma region WolfColor
		SetupValue(Vars::Color::WolfColor[0], 255.f, "Color", "WolfColorR");
		SetupValue(Vars::Color::WolfColor[1], 255.f, "Color", "WolfColorG");
		SetupValue(Vars::Color::WolfColor[2], 255.f, "Color", "WolfColorB");
#pragma endregion
#pragma region HorseColor
		SetupValue(Vars::Color::HorseColor[0], 255.f, "Color", "HorseColorR");
		SetupValue(Vars::Color::HorseColor[1], 255.f, "Color", "HorseColorG");
		SetupValue(Vars::Color::HorseColor[2], 255.f, "Color", "HorseColorB");
#pragma endregion
#pragma region ChickenColor
		SetupValue(Vars::Color::ChickenColor[0], 255.f, "Color", "ChickenColorR");
		SetupValue(Vars::Color::ChickenColor[1], 255.f, "Color", "ChickenColorG");
		SetupValue(Vars::Color::ChickenColor[2], 255.f, "Color", "ChickenColorB");
#pragma endregion
#pragma region BoarColor
		SetupValue(Vars::Color::BoarColor[0], 255.f, "Color", "BoarColorR");
		SetupValue(Vars::Color::BoarColor[1], 255.f, "Color", "BoarColorG");
		SetupValue(Vars::Color::BoarColor[2], 255.f, "Color", "BoarColorB");
#pragma endregion
#pragma region BearsColor
		SetupValue(Vars::Color::BearsColor[0], 255.f, "Color", "BearsColorR");
		SetupValue(Vars::Color::BearsColor[1], 255.f, "Color", "BearsColorG");
		SetupValue(Vars::Color::BearsColor[2], 255.f, "Color", "BearsColorB");
#pragma endregion
#pragma region DeerColor
		SetupValue(Vars::Color::DeerColor[0], 255.f, "Color", "DeerColorR");
		SetupValue(Vars::Color::DeerColor[1], 255.f, "Color", "DeerColorG");
		SetupValue(Vars::Color::DeerColor[2], 255.f, "Color", "DeerColorB");
#pragma endregion

	}

	std::string szPath = "";

protected:
	std::vector<VarType<int>*> ints;
	std::vector<VarType<bool>*> bools;
	std::vector<VarType<float>*> floats;
};

CConfig config;