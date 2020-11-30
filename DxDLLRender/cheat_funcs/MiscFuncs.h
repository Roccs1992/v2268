void Misc()
{
	

	if (Vars::Misc::HighJump) {
		if (FC(user32, GetAsyncKeyState, VK_MBUTTON))
			LocalPlayer.BasePlayer->SetGravity(Vars::Misc::JumpValue);
		else LocalPlayer.BasePlayer->SetGravity(3.f);
	}

	//boost speed on water
	if (Vars::Misc::WaterBoost)
		LocalPlayer.BasePlayer->WaterBoost();


	//boost speed on heavy armor
	if (Vars::Misc::NoHeavyReduct)
		LocalPlayer.BasePlayer->NoHeavyReduct();

	//spider man
	if (Vars::Misc::SpiderMan)
		LocalPlayer.BasePlayer->SpiderMan();

	if (Vars::Misc::AirStuck ) {
		if (FC(user32, GetAsyncKeyState, 0x58))
			LocalPlayer.BasePlayer->AirStuck(true);
		else LocalPlayer.BasePlayer->AirStuck(false);
	}

	//remove weapon sway
	if (Vars::Weapon::NoSway)
		LocalPlayer.BasePlayer->NoSway();

	if (Vars::Misc::FakeAdmin)
		LocalPlayer.BasePlayer->FakeAdmin(4);

	if (Vars::Misc::WalkOnWater && GetAsyncKeyState(0x58))
		LocalPlayer.BasePlayer->WalkWater();

	if (Vars::Misc::NoBlockAiming)
		LocalPlayer.BasePlayer->NoBlockAiming();

	if (Vars::Misc::CustomFov)
		LocalPlayer.BasePlayer->SetFov();

	if (Vars::Misc::SilentWalk)
		LocalPlayer.BasePlayer->SetRemoveFlag(4);
}

