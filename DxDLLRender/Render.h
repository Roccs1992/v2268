#include <d2d1.h>
#include <dwrite_1.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


//render
class RenderClass
{
public:
	ID2D1RenderTarget* Canvas;
	IDWriteFactory1* TextEngine;
	IDWriteTextFormat* TextProp;

	//rectangle
	void Rectangle(const Vector2& Dot, const Vector2& WidthHeight, const D2D1::ColorF& Clr, float Thickness = 1.5f)
	{
		ID2D1SolidColorBrush* BClr;
		Canvas->CreateSolidColorBrush(Clr, &BClr);
		Canvas->DrawRectangle({ Dot.x, Dot.y, Dot.x + WidthHeight.x, Dot.y + WidthHeight.y }, BClr, Thickness);
		BClr->Release();
	}

	void FillRectangle(const Vector2& Dot, const Vector2& WidthHeight, const D2D1::ColorF& Clr)
	{
		ID2D1SolidColorBrush* BClr;
		Canvas->CreateSolidColorBrush(Clr, &BClr);
		Canvas->FillRectangle({ Dot.x, Dot.y, Dot.x + WidthHeight.x, Dot.y + WidthHeight.y }, BClr);
		BClr->Release();
	}

	void RoundedRectangle(const Vector2& Dot, const Vector2& WidthHeight, const D2D1::ColorF& Clr, float Radius, float Thickness = 1.5f)
	{
		ID2D1SolidColorBrush* BClr;
		Canvas->CreateSolidColorBrush(Clr, &BClr);
		Canvas->DrawRoundedRectangle({ {Dot.x, Dot.y, Dot.x + WidthHeight.x, Dot.y + WidthHeight.y }, Radius, Radius }, BClr, Thickness);
		BClr->Release();
	}

	void FillRoundedRectangle(const Vector2& Dot, const Vector2& WidthHeight, const D2D1::ColorF& Clr, float Radius)
	{
		ID2D1SolidColorBrush* BClr;
		Canvas->CreateSolidColorBrush(Clr, &BClr);
		Canvas->FillRoundedRectangle({ {Dot.x, Dot.y, Dot.x + WidthHeight.x, Dot.y + WidthHeight.y}, Radius, Radius }, BClr);
		BClr->Release();
	}

	void LGBT_FillRoundedRectangle(const Vector2& Dot, const Vector2& WidthHeight, const D2D1::ColorF& Clr, float Radius, float per)
	{
		//ID2D1SolidColorBrush* BClr;

		auto kek_g = 1.f - per; auto kek_r = 1.f - kek_g;
		ID2D1GradientStopCollection* pGradientStops = NULL;

		D2D1_GRADIENT_STOP gradientStops[4];
		gradientStops[0].color = D2D1::ColorF{ D2D1::ColorF::Red };
		gradientStops[0].position = 0.f;

		gradientStops[1].color = D2D1::ColorF{ D2D1::ColorF::Red };
		gradientStops[1].position = per;

		//gradientStops[1].color = D2D1::ColorF{ kek_r,0,0, 1 };
		//gradientStops[1].position = 0.5f;

		gradientStops[2].color = D2D1::ColorF{ D2D1::ColorF::Green };
		gradientStops[2].position = per;

		gradientStops[3].color = D2D1::ColorF{ D2D1::ColorF::Green };
		gradientStops[3].position = 1.f;
		//D2D1_RECT_F flex = { Dot.x + WidthHeight.x,  Dot.y + WidthHeight.y };

		//ID2D1SolidColorBrush* BClr;
		//Canvas->CreateSolidColorBrush(Clr, &BClr);

		//Canvas->FillRectangle(&flex, BClr);
		//Canvas->DrawRectangle(&flex, BClr, 1, NULL);

		//BClr->Release();


		// Create the ID2D1GradientStopCollection from a previously
		// declared array of D2D1_GRADIENT_STOP structs.
		Canvas->CreateGradientStopCollection(
			gradientStops,
			4,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_WRAP,
			&pGradientStops
		);

		ID2D1LinearGradientBrush* m_pLinearGradientBrush;
		Canvas->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(Dot.x, Dot.y),
				D2D1::Point2F(Dot.x + WidthHeight.x, Dot.y + WidthHeight.y)),
			pGradientStops,
			&m_pLinearGradientBrush
		);


		Canvas->FillRoundedRectangle({ {Dot.x, Dot.y, Dot.x + WidthHeight.x, Dot.y + WidthHeight.y}, Radius, Radius }, m_pLinearGradientBrush);
		m_pLinearGradientBrush->Release();
		pGradientStops->Release();
	}


	//circle
	void Circle(const Vector2& Dot, const D2D1::ColorF& Clr, float Radius, float Thickness = 1.5f)
	{
		ID2D1SolidColorBrush* BClr;
		Canvas->CreateSolidColorBrush(Clr, &BClr);
		Canvas->DrawEllipse({ { Dot.x, Dot.y }, Radius, Radius }, BClr, Thickness);
		BClr->Release();
	}

	void FillCircle(const Vector2& Dot, const D2D1::ColorF& Clr, float Radius, bool Left = false)
	{
		ID2D1SolidColorBrush* BClr;
		Canvas->CreateSolidColorBrush(Clr, &BClr);
		Canvas->FillEllipse({ { !Left ? Dot.x + Radius : Dot.x - Radius, Dot.y /*+ Radius*/ }, Radius, Radius }, BClr);
		BClr->Release();
	}

	//line
	void Line(const Vector2& Dot1, const Vector2& Dot2, const D2D1::ColorF& Clr, float Thickness = 1.5f)
	{
		ID2D1SolidColorBrush* BClr;
		Canvas->CreateSolidColorBrush(Clr, &BClr);
		Canvas->DrawLine({ Dot1.x, Dot1.y }, { Dot2.x, Dot2.y }, BClr, Thickness);
		BClr->Release();
	}

	//text
	Vector2 String(const Vector2& Dot, const std::wstring& Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White))
	{
		ID2D1SolidColorBrush* BClr; Canvas->CreateSolidColorBrush(Clr, &BClr); IDWriteTextLayout* TextLayout;
		TextEngine->CreateTextLayout(Str.c_str(), (UINT)Str.length(), TextProp, 1000.f, 1000.f, &TextLayout);
		DWRITE_TEXT_METRICS TextSize; TextLayout->GetMetrics(&TextSize);
		Canvas->DrawTextLayout({ Dot.x, Dot.y }, TextLayout, BClr, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
		BClr->Release(); TextLayout->Release(); return { TextSize.width, TextSize.height };
	}

	Vector2 StringCenter(const Vector2& Dot, const std::wstring& Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White))
	{
		ID2D1SolidColorBrush* BClr; DWRITE_TEXT_METRICS TextSize;
		Canvas->CreateSolidColorBrush(Clr, &BClr); IDWriteTextLayout* TextLayout;
		TextEngine->CreateTextLayout(Str.c_str(), (UINT)Str.length(), TextProp, 1000.f, 1000.f, &TextLayout);
		TextLayout->GetMetrics(&TextSize); Vector2 HalfSize = { (TextSize.width / 2),  (TextSize.height / 2) };
		Canvas->DrawTextLayout({ Dot.x - HalfSize.x, Dot.y - HalfSize.y }, TextLayout, BClr, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
		BClr->Release(); TextLayout->Release(); return HalfSize;
	}
};

typedef void(__fastcall RenderFn)(RenderClass*);
