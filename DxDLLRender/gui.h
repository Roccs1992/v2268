#include <d2d1.h>
#include <dwrite_1.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

namespace GUI
{
	WNDPROC NextWndProc;
	enum Button {
		NoPress,
		Clicked,
		Pressed
	};
	struct IO {
		Button LKM;
		Vector2 MousePos;
		Vector2 MouseDelta;
		USHORT CurElement;
	} IO;
	struct WndData {
		Vector2 Size;
		Vector2 NextPos;
	} CurWnd;

	//fast defs
	#define Clamp(Val, Min, Max) ((Val > Max) ? Max : ((Val < Min) ? Min : Val))

	#define OffInWnd() (IO.MousePos - CurWnd.NextPos)
	
	//str hash
	unsigned short __forceinline HashStr(const wchar_t* Str)
	{
		unsigned char i;
		unsigned short crc = 0xFFFF;
		while (wchar_t DChar = *Str++) {
			unsigned char Char = (unsigned char)DChar;
			crc ^= Char << 8;
			for (i = 0; i < 8; i++)
				crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
			Char = (unsigned char)(DChar >> 8);
			crc ^= Char << 8;
			for (i = 0; i < 8; i++)
				crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
		} return crc;
	}

	Vector2 __forceinline CenterLine(const Vector2& Pos) {
		return { (Pos.x + (CurWnd.Size.x / 2)), Pos.y };
	}

	bool __forceinline InRect(Vector2 Rect, Vector2 Size, Vector2 Dot) {
		return (Dot.x > Rect.x && Dot.x < Rect.x + Size.x && Dot.y > Rect.y && Dot.y < Rect.y + Size.y);
	}

	//input
	LRESULT WINAPI WndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
			case WM_LBUTTONDOWN:
				IO.LKM = Clicked;
				break;

			case WM_LBUTTONUP:
				IO.LKM = NoPress;
				IO.CurElement = 0;
				break;

			case WM_MOUSEMOVE:
				IO.MousePos.x = (signed short)(lParam);
				IO.MousePos.y = (signed short)(lParam >> 16);
				break;

			case WM_KEYUP:
				if (wParam == VK_NEXT)
				{
					Vars::Global::MenuVisible = !Vars::Global::MenuVisible;

					//if (!Vars::Global::MenuVisible)
						//config.Save();
				}

				else if (wParam == VK_HOME)
				{
					if (Vars::Global::MenuVisible)
						Vars::Global::MenuVisible = false;

					Vars::Global::Panic = true;
					Vars::AimBot::Activate = false;

					Vars::Visuals::ShowPlayerBox = false;
					Vars::Visuals::ShowPlayerName = false;
					Vars::Visuals::ShowPlayerHealth = false;
					Vars::Visuals::ShowPlayerWeapon = false;
					Vars::Visuals::ShowPlayerDist = false;
					Vars::Visuals::IgnoreSleepers = false;
					Vars::Visuals::AlwaysDay = false;
					Vars::Visuals::Crosshair = false;

					Vars::Misc::AntiSpread = false;
					Vars::Misc::FastReload = false;
					Vars::Misc::Automatic = false;
					Vars::Misc::SuperEoka = false;
					Vars::Misc::SkinChanger = false;
					Vars::Misc::NoSway = false;
					Vars::Misc::SuperBow = false;
					Vars::Misc::FakeAdmin = false;
					Vars::Misc::HighJump = false;
					Vars::Misc::WaterBoost = false;
					Vars::Misc::NoFall = false;
					Vars::Misc::AirStuck = false;
					Vars::Misc::AntiKastrulya = false;
					Vars::Misc::NoHeavyReduct = false;
					Vars::Misc::BuildAsUWant = false;
					Vars::Misc::LongHand = false;
				}
		}
		return CallWindowProc(NextWndProc, Wnd, Msg, wParam, lParam);
	}

	bool InputMgr(const wchar_t* Name, bool Reg = false)
	{
		unsigned short StrHash = HashStr(Name);
		if (Reg && !IO.CurElement) IO.CurElement = StrHash;
		else if (!Reg) return (IO.CurElement == StrHash);
		return false;
	}

	void ProcessInput(bool End = false) 
	{
		if (!End) 
		{
			//calc mouse delta
			static Vector2 OldMousePos;
			IO.MouseDelta = IO.MousePos - OldMousePos;
			OldMousePos = IO.MousePos;
		}

		else
		{
			//update LKM button
			if (IO.LKM == Clicked)
				IO.LKM = Pressed;
		}
	}

	//main
	void PastTitle(const wchar_t* Name, RenderClass* Render)
	{
		CurWnd.NextPos.y += 5.f;
		Vector2 CntLine = CenterLine(CurWnd.NextPos);
		Vector2 TextSize = Render->StringCenter(CntLine, Name);
		Render->Line({ CntLine.x - TextSize.x - 2.f, CntLine.y }, { CurWnd.NextPos.x + 4.f, CurWnd.NextPos.y }, D2D1::ColorF::White);
		Render->Line({ CntLine.x + TextSize.x + 2.f, CntLine.y }, { CurWnd.NextPos.x + CurWnd.Size.x - 4.f, CurWnd.NextPos.y }, D2D1::ColorF::White);
		CurWnd.NextPos.y += 10.f;
	}

	void CheckBox(const wchar_t* Name, bool* Switch, RenderClass* Render)
	{
		bool SW = *Switch; Render->String({ CurWnd.NextPos.x + 42.f, CurWnd.NextPos.y }, (wchar_t*)Name);
		if (IO.LKM == Clicked && InRect({ CurWnd.NextPos.x + 10.f, CurWnd.NextPos.y }, { 30.f, 16.f }, IO.MousePos)) {
			SW = !SW; InputMgr(Name, 1);
		}

		Render->FillRoundedRectangle({ CurWnd.NextPos.x + 10.f, CurWnd.NextPos.y }, { 30.f, 16.f }, SW ? D2D1::ColorF::LimeGreen : D2D1::ColorF::Red, 8.f);
		Render->FillCircle({ CurWnd.NextPos.x + 10.f + (SW ? 14.f : 0.f), CurWnd.NextPos.y + 8.f }, D2D1::ColorF::White, 8.f); *Switch = SW;
		CurWnd.NextPos.y += 20.f;
	}

	void SliderF(const wchar_t* Name, float* Current, float Min, float Max, RenderClass* Render)
	{
		Render->String({ CurWnd.NextPos.x + 10.f, CurWnd.NextPos.y}, std::wstring((std::wstring)Name + L": " + std::to_wstring(*Current)).c_str());
		float CurOff = *Current / (Max - Min); float ZeroPos = abs(Min) / abs(Max); float CurOffset = *Current / Max - Min;
		CurWnd.NextPos.y += 20.f;

		if (InputMgr(Name)) {
			float kek = (Max - Min) / (CurWnd.Size.x - 36.f);
			*Current = Clamp(((OffInWnd().x - 18.f) - ((CurWnd.Size.x - 36.f) *  ZeroPos)) * kek, Min, Max);
		}

		else if (IO.LKM == Clicked && InRect({ (CurWnd.NextPos.x + 10.f), CurWnd.NextPos.y }, { CurWnd.Size.x - 20.f, 16.f }, IO.MousePos))
			InputMgr(Name, 1);

		Render->FillRoundedRectangle({ CurWnd.NextPos.x + 10.f, CurWnd.NextPos.y }, { CurWnd.Size.x - 20.f, 16.f }, { 0.844f, 0.844f, 0.844f, 0.7f }, 8.f);
		Render->FillCircle({ CurWnd.NextPos.x + 10.f + ((CurWnd.Size.x - 36.f) * CurOff + ((CurWnd.Size.x - 36.f) * ZeroPos)), CurWnd.NextPos.y + 8 }, D2D1::ColorF::White, 8.f);
		CurWnd.NextPos.y += 20.f;
	}

	void Begin(const wchar_t* Name, Vector2& Pos, const const Vector2& Size, const D2D1::ColorF& Clr, RenderClass* Render)
	{
		//drag window
		if (InputMgr(Name)) Pos += IO.MouseDelta;
		else if (IO.LKM == Pressed && InRect(Pos, Size, IO.MousePos))
			InputMgr(Name, 1);
	
		//base menu
		CurWnd.Size = Size;
		CurWnd.NextPos.x = Pos.x; 
		CurWnd.NextPos.y = Pos.y + 25.f;
		Render->FillRoundedRectangle(Pos, Size, Clr, 8.f);
		Render->StringCenter(CenterLine({ Pos.x, Pos.y + 10.f }), (wchar_t*)Name);
	}
}


void BeginRender(IDXGISwapChain* SwapChain, RenderFn CallBack)
{
	static IDWriteFactory1* TextEngine;
	static IDWriteTextFormat* TextProp;
	RenderClass RenderContext;

	//set d3d flags
	ID3D11Device* d3d_device;
	if (SwapChain->GetDevice(IID_PPV_ARGS(&d3d_device))) return;
	WORD FlagsOffset = *(WORD*)((*(DWORD64 * *)d3d_device)[38] + 2); //x64
	*(INT*)((DWORD64)d3d_device + FlagsOffset) |= 0x20; //BGRA_SUPPORT
	d3d_device->Release();

	//get d3d backbuffer
	IDXGISurface* d3d_bbuf;
	if (SwapChain->GetBuffer(0, IID_PPV_ARGS(&d3d_bbuf)))
		return;

	//create d2d
	ID2D1Factory* d2d_instance;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, D2D1_FACTORY_OPTIONS{ D2D1_DEBUG_LEVEL_NONE }, &d2d_instance);

	//prep render target
	const D2D1_RENDER_TARGET_PROPERTIES d2d_prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT state = d2d_instance->CreateDxgiSurfaceRenderTarget(d3d_bbuf, d2d_prop, &RenderContext.Canvas); d2d_instance->Release(); d3d_bbuf->Release(); if (state) return;

	//prep font engine
	if (!TextProp) {
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(TextEngine), (IUnknown * *)(&TextEngine));
		TextEngine->CreateTextFormat(L"Tahoma", NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 12.f, L"", &TextProp);
	} RenderContext.TextEngine = TextEngine; RenderContext.TextProp = TextProp;

	GUI::ProcessInput();

	//draw scene
	RenderContext.Canvas->BeginDraw();
	CallBack(&RenderContext);
	RenderContext.Canvas->EndDraw();
	RenderContext.Canvas->Release();

	GUI::ProcessInput(true);
}
