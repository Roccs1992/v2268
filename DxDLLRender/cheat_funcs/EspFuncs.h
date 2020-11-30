static D2D1::ColorF misc_flags(0.23f, 1.0f, 1.0f, 1.0f);

#include <d3d9.h>
#include <iostream>
#include <wingdi.h>
D3DCOLOR FLOAT4TOD3DCOLOR(float Col[])
{
	ImU32 col32_no_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(Col[0], Col[1], Col[2], Col[3]));
	float a = (col32_no_alpha >> 24) & 255;
	float r = (col32_no_alpha >> 16) & 255;
	float g = (col32_no_alpha >> 8) & 255;
	float b = col32_no_alpha & 255;
	return D3DCOLOR_ARGB((int)a, (int)r, (int)g, (int)b);
}

D3DCOLOR FLOAT4TOD3DCOLOR2(float Col[])
{
	ImU32 col32_no_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(Col[0], Col[1], Col[2], Col[3]));
	float a = (col32_no_alpha >> 24) & 255;
	float r = (col32_no_alpha >> 16) & 255;
	float g = (col32_no_alpha >> 8) & 255;
	float b = col32_no_alpha & 255;
	return D3DCOLOR_ARGB((int)a, (int)b, (int)g, (int)r);
}

void Skeleton(BasePlayer* BasePlayer)
{
	const ImColor ColorNeckband = FLOAT4TOD3DCOLOR(Vars::Color::SkeletonColors);
	BoneList Bones[15] = {
		/*LF*/l_foot, l_knee, l_hip,
		/*RF*/r_foot, r_knee, r_hip,
		/*BD*/spine1, neck, head,
		/*LH*/l_upperarm, l_forearm, l_hand,
		/*RH*/r_upperarm, r_forearm, r_hand
	}; Vector2 BonesPos[15];

	//get bones screen pos
	for (int j = 0; j < 15; j++) {
		if (!LocalPlayer.WorldToScreen(BasePlayer->GetBoneByID(Bones[j]), BonesPos[j]))
			return;
	}

	//draw main lines
	for (int j = 0; j < 15; j += 3) {
		GUI::Render.DrawLine(ImVec2{ BonesPos[j].x, BonesPos[j].y }, ImVec2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, ColorNeckband);
		GUI::Render.DrawLine(ImVec2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, ImVec2{ BonesPos[j + 2].x, BonesPos[j + 2].y }, ColorNeckband);
	}

	//draw add lines
	GUI::Render.DrawLine(ImVec2{ BonesPos[2].x, BonesPos[2].y }, ImVec2{ BonesPos[6].x, BonesPos[6].y }, ColorNeckband);
	GUI::Render.DrawLine(ImVec2{ BonesPos[5].x, BonesPos[5].y }, ImVec2{ BonesPos[6].x, BonesPos[6].y }, ColorNeckband);
	GUI::Render.DrawLine(ImVec2{ BonesPos[9].x, BonesPos[9].y }, ImVec2{ BonesPos[7].x, BonesPos[7].y }, ColorNeckband);
	GUI::Render.DrawLine(ImVec2{ BonesPos[12].x, BonesPos[12].y }, ImVec2{ BonesPos[7].x, BonesPos[7].y }, ColorNeckband);
}

void MemeBox(const ImColor& PlayerClr, BasePlayer* BasePlayer)
{
	BoneList Bones[4] = {
		/*UP*/l_upperarm, r_upperarm,
		/*DOWN*/r_foot, l_foot
	}; Vector2 BonesPos[4];

	//get bones screen pos
	for (int j = 0; j < 4; j++) {
		if (!LocalPlayer.WorldToScreen(BasePlayer->GetBoneByID(Bones[j]), BonesPos[j]))
			return;
	}

	//draw main lines
	GUI::Render.DrawLine(ImVec2{ BonesPos[0].x, BonesPos[0].y }, ImVec2{ BonesPos[1].x, BonesPos[1].y }, PlayerClr);
	GUI::Render.DrawLine(ImVec2{ BonesPos[0].x, BonesPos[0].y }, ImVec2{ BonesPos[3].x, BonesPos[3].y }, PlayerClr);
	GUI::Render.DrawLine(ImVec2{ BonesPos[2].x, BonesPos[2].y }, ImVec2{ BonesPos[1].x, BonesPos[1].y }, PlayerClr);
	GUI::Render.DrawLine(ImVec2{ BonesPos[2].x, BonesPos[2].y }, ImVec2{ BonesPos[3].x, BonesPos[3].y }, PlayerClr);
}

//Crosshair
void Crosshair()
{
	float xs = Vars::Global::ScreenWidth / 2, ys = Vars::Global::ScreenHigh / 2;

	GUI::Render.DrawLine(ImVec2{ xs, ys - 4 }, ImVec2{ xs , ys + 5 },  FLOAT4TOD3DCOLOR(Vars::Color::CrosshairColors));
	GUI::Render.DrawLine(ImVec2{ xs - 4, ys }, ImVec2{ xs + 5, ys },  FLOAT4TOD3DCOLOR(Vars::Color::CrosshairColors));
	
}

//Fov
void DrawFOV()
{
	float xs = Vars::Global::ScreenWidth / 2, ys = Vars::Global::ScreenHigh / 2;

	if (Vars::AimBot::Fov) {
		GUI::Render.DrawCircle(ImVec2{ xs , ys }, Vars::AimBot::Fov, FLOAT4TOD3DCOLOR(Vars::Color::Fov));

		GUI::Render.FillCircle(Vector2{ xs , ys }, D2D1::ColorF(0.f, 0.f, 0.f, 0.3f), Vars::AimBot::Fov / Vars::AimBot::FillFov);

	}

}



void ESP(BasePlayer* BP, BasePlayer* LP, bool npc = false)
{
	bool PlayerSleeping = BP->HasFlags(16);
	bool PlayerWounded = BP->HasFlags(16);
	if (Vars::PlayerEsp::IgnoreSleepers && PlayerSleeping)
		if (Vars::PlayerEsp::PlayerWounded && PlayerWounded)
			return;

	if (Vars::PlayerEsp::Skeleton)
		Skeleton(BP);

	Vector2 tempFeetR, tempFeetL;
	Vector3 ro = BP->GetBoneByID(r_foot);
	if (ro.x == 0 && ro.y == 0 && ro.z == 0) return;
	if (LocalPlayer.WorldToScreen(ro, tempFeetR) && LocalPlayer.WorldToScreen(BP->GetBoneByID(l_foot), tempFeetL))
	{
		Vector2 tempHead;
		if (LocalPlayer.WorldToScreen(BP->GetBoneByID(jaw) + Vector3(0.f, 0.16f, 0.f), tempHead))
		{
			Vector2 tempFeet = (tempFeetR + tempFeetL) / 2.f;
			float Entity_h = tempHead.y - tempFeet.y;
			float w = Entity_h / 4;
			float Entity_x = tempFeet.x - w;
			float Entity_y = tempFeet.y;
			float Entity_w = Entity_h / 2;

			bool PlayerWounded = BP->HasFlags(64);
			auto PlayerClr = !npc ? (PlayerSleeping ? FLOAT4TOD3DCOLOR(Vars::Color::SleepersColor) : (PlayerWounded ? FLOAT4TOD3DCOLOR(Vars::Color::WoundedColor) : FLOAT4TOD3DCOLOR(Vars::Color::BoxColors))) : FLOAT4TOD3DCOLOR(Vars::Color::BotBoxColors);
			if (Vars::PlayerEsp::ShowPlayerBox) {
				if (!PlayerWounded && !PlayerSleeping) {
					GUI::Render.DrawBorder(ImVec2{ Entity_x, Entity_y }, ImVec2{ Entity_w, Entity_h }, PlayerClr, 0, 0, 1);
					GUI::Render.DrawBorder(ImVec2{ Entity_x, Entity_y }, ImVec2{ Entity_w, Entity_h }, PlayerClr, 0, 0, 1);

					if (Vars::PlayerEsp::FillBox) {
						GUI::Render.FillARGB(ImVec2{ Entity_x, Entity_y }, ImVec2{ Entity_w, Entity_h }, FLOAT4TOD3DCOLOR(Vars::Color::BoxFilledColor), 0, 0);
					}
				}
				else {
					MemeBox(FLOAT4TOD3DCOLOR(Vars::Color::SleepersColor), BP); //sleepers
				}
			}

			if (Vars::PlayerEsp::SnapLines)
			{
				static float screenX = Vars::Global::ScreenWidth;
				static float screenY = Vars::Global::ScreenHigh;
				static ImVec2 startPos = ImVec2(screenX / 2, screenY);

				GUI::Render.DrawLine(startPos, ImVec2{ Entity_x + Entity_w / 2.5f, Entity_y }, FLOAT4TOD3DCOLOR(Vars::Color::SnapColors));
			}

			int CurPos = 0;

			if (Vars::PlayerEsp::ShowPlayerName || Vars::PlayerEsp::ShowPlayerDist) {
				wchar_t res[64];
				if (wcslen(BP->GetName()) > 0) {
					if (Vars::PlayerEsp::ShowPlayerName && Vars::PlayerEsp::ShowPlayerDist) {
						_swprintf(res, L"%s [%d M]", BP->GetName(), (int)Math::Calc3D_Dist(LP->GetBoneByID(head), ro));
					}
					else if (Vars::PlayerEsp::ShowPlayerDist) {
						_swprintf(res, L"%d m", (int)Math::Calc3D_Dist(LP->GetBoneByID(head), ro));
					}
					else {
						_swprintf(res, L"%s", BP->GetName());
					}
					GUI::Render.String(Vector2{ Entity_x + Entity_w, Entity_y + Entity_h - 20.f }, res, FLOAT4TOD3DCOLOR2(Vars::Color::PlayerRenderStringColor));
				}
			}

			if (Vars::PlayerEsp::ShowPlayerHealth) {
				int health = (int)BP->GetHealth();
				float maxheal = (npc ? 300.f : 100.f);
				GUI::Render.FillARGB(ImVec2{ Entity_x + Entity_w - 8.f, Entity_y }, ImVec2{ 5, Entity_h }, FLOAT4TOD3DCOLOR(Vars::Color::EmptyHpBar), 0, 0);
				GUI::Render.FillARGB(ImVec2{ Entity_x + Entity_w - 8.f, Entity_y }, ImVec2{ 5, Entity_h * (health / maxheal) }, FLOAT4TOD3DCOLOR(Vars::Color::HpBar), 0, 0);
				GUI::Render.DrawBorder(ImVec2{ Entity_x + Entity_w - 8.f, Entity_y }, ImVec2{ 5, Entity_h }, ImColor(0, 0, 0, 0), 0.5f, 0, 1);
			}

			if (Vars::PlayerEsp::ShowPlayerWeapon)
			{
				WeaponData* ActWeapon = BP->GetActiveWeapon();
				if (ActWeapon)
				{
					const wchar_t* ActiveWeaponName;
					ActiveWeaponName = ActWeapon->GetName();
					GUI::Render.String(Vector2{ (Entity_x + 7), (Entity_y + Entity_h + CurPos) }, ActiveWeaponName, FLOAT4TOD3DCOLOR2(Vars::Color::WeaponColors));
					CurPos += 15;
				}
			}
		}
	}
}