#include <d2d1.h>
#include <dwrite_1.h>
#include <intrin.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
class RenderClass
{
private:
	//render context
	ID2D1Factory* Interface;
	ID2D1RenderTarget* Canvas;
	IDWriteFactory1* TextEngine;
	IDWriteTextFormat* TextFormat;
	IDWriteTextFormat* BigTextFormat;
	IDWriteTextFormat* CuTextFormat;
	ID2D1SolidColorBrush* SolidColor;

	//fast get wstring length
	__forceinline UINT wcslen(const wchar_t* Str) {
		const wchar_t* TempStr = Str;
		for (; *TempStr; ++TempStr);
		return (UINT)(TempStr - Str);
	}

	__declspec(noinline) bool InitRender(IDXGISwapChain* SwapChain)
	{
		//prep d2d render & text engine
		static bool initialized; if (!initialized) {
			initialized = true; D2D1_FACTORY_OPTIONS CreateOpt = { D2D1_DEBUG_LEVEL_NONE };
			FC(dwrite, DWriteCreateFactory, DWRITE_FACTORY_TYPE_SHARED, __uuidof(TextEngine), (IUnknown**)&TextEngine);
			FC(d2d1, D2D1CreateFactory, D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &CreateOpt, (void**)&Interface);
			TextEngine->CreateTextFormat(StrW(L"Bold Script"), NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 12.f, L"", &TextFormat);
			TextEngine->CreateTextFormat(StrW(L"Bold Script"), NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 20.f, L"", &BigTextFormat);
			TextEngine->CreateTextFormat(StrW(L"Bold Script"), NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 20.f, L"", &CuTextFormat);
			printf("InitRender %d %d %d\n", Interface != 0, TextEngine != 0, TextFormat != 0);
			if (!Interface || !TextEngine || !TextFormat) return false;
		}

		//get window flags var
		ID3D11Device* d3d_device;
		if (SwapChain->GetDevice(IID_PPV_ARGS(&d3d_device))) return false;
		WORD flagsOffset = *(WORD*)((*(DWORD64**)d3d_device)[38] + 2); //x64
		int& flags = *(INT*)((DWORD64)d3d_device + flagsOffset);
		d3d_device->Release();

		//get d3d backbuffer
		IDXGISurface* d3d_bbuf;
		if (SwapChain->GetBuffer(0, IID_PPV_ARGS(&d3d_bbuf)))
			return false;

		//create canvas & cleanup
		D2D1_RENDER_TARGET_PROPERTIES d2d_prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
		flags |= 0x20; HRESULT canvas_state = Interface->CreateDxgiSurfaceRenderTarget(d3d_bbuf, d2d_prop, &Canvas); flags &= ~0x20; d3d_bbuf->Release(); if (canvas_state) return false;
		if (!SolidColor) Canvas->CreateSolidColorBrush({}, &SolidColor); return true;
	}

public:
	//canvas mgr
	__forceinline bool NewFrame(IDXGISwapChain* SwapChain)
	{
		//need prep d2d canvas
		if (!Canvas && !InitRender(SwapChain))
			return false;

		//draw start
		Canvas->BeginDraw();
		return true;
	}

	__forceinline Vector2 CanvasSize()
	{
		D2D1_SIZE_F Size = Canvas->GetSize();
		return Vector2{ Size.width, Size.height };
	}

	__forceinline void ResetCanvas()
	{
		if (Canvas)
		{
			Canvas->Release();
			Canvas = nullptr;
		}
	}

	__forceinline void EndFrame()
	{
		HRESULT state = Canvas->EndDraw();
		if (state == D2DERR_RECREATE_TARGET)
			ResetCanvas();
	}

	//line
	__forceinline void DrawLine(const ImVec2& from, const ImVec2& to, ImColor color/*ARGB*/)
	{
		ImGui::GetBackgroundDrawList()->AddLine(from, to, color, 1.0f);
	}

	//circle
	void DrawCircle(const ImVec2& centre, float radius, ImColor color/*ARGB*/)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(centre, radius, color, 32);
	}

	__forceinline void FillCircle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad)
	{
		SolidColor->SetColor(Clr); Canvas->FillEllipse({ { Start.x, Start.y }, Rad, Rad }, SolidColor);
	}

	//rectangle
	void DrawBorder(const ImVec2& pos_a, const ImVec2& pos_b, ImColor color, float rounding/*=0.0f*/, int rounding_corners /*=-1*/, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddRect(pos_a, { pos_a.x + pos_b.x, pos_a.y + pos_b.y }, color, 0.0f, rounding_corners, thickness);
	}

	void FillARGB(const ImVec2& pos_a, const ImVec2& pos_b, ImColor color, float rounding/*=0.0f*/, int rounding_corners /*=-1*/)
	{
		ImGui::GetBackgroundDrawList()->AddRectFilled(pos_a, { pos_a.x + pos_b.x, pos_a.y + pos_b.y }, color, 0.0f, rounding_corners);
	}

	void DrawHealthBox(int x, int y, ImColor m_dColorOut, ImColor m_dColorIn, int m_iHealth, int m_iMaxHealth)
	{
		if (m_iHealth == 0)
			m_iHealth = 1;
		float mx = (float)m_iMaxHealth / 2;
		float w = (float)m_iHealth / 2;
		x -= ((int)mx / 2);

		FillARGB(ImVec2(x, y), ImVec2(mx, 4), m_dColorOut, 0.0f, -1);
		FillARGB(ImVec2(x, y), ImVec2(w, 4), m_dColorIn, 0.0f, -1);

		DrawBorder(ImVec2(x - 1, y - 1), ImVec2(mx + 2, 6), ImColor(255, 30, 30, 30), 0.0f, -1, 1.5);

	}

	__forceinline void FillRoundedRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Rad)
	{
		SolidColor->SetColor(Clr); Canvas->FillRoundedRectangle({ {Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y}, Rad, Rad }, SolidColor);
	}

	//text
	__forceinline Vector2 StringCenter(const Vector2& Start, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool big = false)
	{
		SolidColor->SetColor(Clr); IDWriteTextLayout* TextLayout;
		if (big) TextEngine->CreateTextLayout(Str, this->wcslen(Str), CuTextFormat, 1000.f, 200.f, &TextLayout);
		else TextEngine->CreateTextLayout(Str, this->wcslen(Str), TextFormat, 200.f, 100.f, &TextLayout);
		DWRITE_TEXT_METRICS TextInfo;
		TextLayout->GetMetrics(&TextInfo);
		Vector2 TextSize = { TextInfo.width / 2.f, TextInfo.height / 2.f };
		Canvas->DrawTextLayout({ Start.x - TextSize.x, Start.y - TextSize.y }, TextLayout, SolidColor); TextLayout->Release();
		return TextSize; //return half text size
	}

	__forceinline void String(const Vector2& Start, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool big = false)
	{
		SolidColor->SetColor(Clr);
		if (!big) Canvas->DrawTextW(Str, this->wcslen(Str), TextFormat, { Start.x, Start.y, FLT_MAX, FLT_MAX }, SolidColor);
		else Canvas->DrawTextW(Str, this->wcslen(Str), BigTextFormat, { Start.x, Start.y, FLT_MAX, FLT_MAX }, SolidColor);
	}
};
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace GUI
{
	bool Visible = true;
	RenderClass Render;
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
		Vector2 OldMousePos;
		USHORT CurElement;
	} IO;
	struct WndData {
		Vector2 WndPos;
		Vector2 Size;
		Vector2 Pos;
	} CurWnd;

	unsigned short __forceinline HashStr(const wchar_t* Str)
	{
		unsigned char i;
		unsigned short crc = 0xFFFF;
		while (wchar_t DChar = *Str++)
		{
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

	Vector2 __forceinline CenterLine(const Vector2& Pos)
	{
		return { (Pos.x + (CurWnd.Size.x / 2)), Pos.y };
	}

	bool __forceinline InRect(Vector2 Rect, Vector2 Size, Vector2 Dot)
	{
		return (Dot.x > Rect.x && Dot.x < Rect.x + Size.x && Dot.y > Rect.y && Dot.y < Rect.y + Size.y);
	}

	LRESULT WINAPI WndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case WM_LBUTTONDOWN:
			IO.LKM = Pressed;
			break;

		case WM_LBUTTONUP:
			IO.LKM = Clicked;
			IO.CurElement = 0;
			break;

		case WM_KEYUP:
			if (wParam == VK_INSERT || wParam == VK_NEXT)
				Visible = !Visible;
			break;

		case WM_MOUSEMOVE:
			IO.MousePos.x = (signed short)(lParam);
			IO.MousePos.y = (signed short)(lParam >> 16);
			break;
		}

		if (Visible && !ImGui_ImplWin32_WndProcHandler(Wnd, Msg, wParam, lParam))
			return TRUE;

		return FC(user32, CallWindowProcW, NextWndProc, Wnd, Msg, wParam, lParam);
	}

	void ProcessInput(bool End = false)
	{
		if (!End)
		{
			//calc mouse delta
			IO.MouseDelta = IO.MousePos - IO.OldMousePos;
			IO.OldMousePos = IO.MousePos;
		}

		else
		{
			//update LKM button
			if (IO.LKM == Clicked)
				IO.LKM = NoPress;
		}
	}

	bool InputMgr(const wchar_t* Name, bool Reg = false)
	{
		unsigned short StrHash = HashStr(Name);
		if (Reg && !IO.CurElement)
		{
			IO.CurElement = StrHash;
			return true;
		}
		else if (!Reg)
			return (IO.CurElement == StrHash);
		return false;
	}

	//main

	void Begin(Vector2& Pos, const Vector2& Size, const D2D1::ColorF& Clr)
	{
		//base menu
		ProcessInput();
		if (!CurWnd.WndPos.Zero()) Pos = CurWnd.WndPos;
		Render.FillRoundedRectangle(Pos, Size, Clr, 18.f);
		CurWnd.WndPos = { Pos.x, Pos.y }; CurWnd.Size = Size;
		CurWnd.Pos = { Pos.x, Pos.y + 25.f };
	}

	void End()
	{
		//drag window
		ProcessInput(true);
		//if (InputMgr(L"##Drag") || (IO.LKM == Pressed && InRect(CurWnd.WndPos, CurWnd.Size, IO.MousePos) && InputMgr(L"##Drag", true)))
			//CurWnd.WndPos += IO.MouseDelta;
	}
}