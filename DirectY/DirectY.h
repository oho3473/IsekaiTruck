#pragma once

#include <windows.h>	//HWND

///d2dheader
#include <d2d1.h>		//D2D
#include <dwrite.h>		//DWrite
#include <wincodec.h>	//WIC
#pragma comment(lib,"d2d1")					//D2D
#pragma comment(lib,"dwrite")				//DWrite
#pragma comment(lib,"windowscodecs.lib")	//WIC

///effectsheader
#include <d2d1_1.h>	//ID2D1Effect
#include <d2d1effects.h>	//D2DEffect
#include<d2d1effects_2.h> //edgeDetection
#pragma comment(lib,"d2d1.lib")	//가우스블러 등등을 없으면 못 씀
#pragma comment(lib,"dxguid.lib")	//가우스블러 등등을 없으면 못 씀

///myheader
#include "MakeMatrix.h"	//Transform
#include "Sprite.h"	//sprite구조체


class PaticleMaintain;

///mathEngine
#pragma comment(lib, "MathEngine.lib")

/// <summary>
/// 글자 정렬
/// </summary>
enum class TextAlignMent
{
	LEFT = 0,
	RIGHT,
	CENTER
};

/// <summary>
/// 그래픽스 엔진
/// 2023.08.13 유승운
/// </summary>
class DirectY
{
public:
	DirectY(HWND hWnd);
	~DirectY();

	///기본 함수
	void BeginDraw();	//렌더 시작
	void EndDraw();	//렌더 끝
	void CleanWindow();	//화면 전부 지우기

	///브러쉬 색 지정
	void SetBrushColor(const float& red, const float& green, const float& blue, const float& alpha);
	void SetBrushColor(const int& red, const int& green, const int& blue, const int& alpha);
	void SetBrushColor(D2D1::ColorF color);

	///기본 도형 그리기
	//----------------------------------------------------------------------
	void DrawLine(Graphics::BaseFigure::Line line, const float thickness = 5.0f);	//선 그리기
	void DrawRectangle(Graphics::BaseFigure::Rectangle rect, const float thickness = 1.0f);	//사각형 그리기
	void DrawCircle(Graphics::BaseFigure::Circle circle, const float thickness = 1.0f);	//원 그리기
	void DrawEllipse(Graphics::BaseFigure::Ellipse ellipse, const float thickness = 1.0f);	//타원 그리기
	void FillRectangle(Graphics::BaseFigure::Rectangle rect, const float thickness = 1.0f);	//가득 찬 사각형 그리기
	void DrawIsometric(Graphics::BaseFigure::IsomatricTile tile, const float thickness = 0.5f);	//4개점 선으로 이어서 그리기
	//----------------------------------------------------------------------

	///문자 출력
	//--------------------------------------------------------------------------
	void DrawText(const WCHAR* command, const float& x, const float& y, const float& fontsize = 20, TextAlignMent alignment = TextAlignMent::LEFT, const WCHAR* msc_fontName = L"Danjo-bold");
	bool DrawText(const int& x, const int& y, const float& fontsize, TextAlignMent alignment, const WCHAR* msc_fontName, const char* pch, ...);	//c타입으로 문자 출력
	DWRITE_TEXT_METRICS GetTextMetrics(const WCHAR* command);
	//--------------------------------------------------------------------------

	///transform
	//--------------------------------------------------------------------------
	void Identity() { m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity()); }		//기저벡터 정상화
	void SetTransform(const D2D1_MATRIX_3X2_F& matrix);	//변환 행렬을 통해 변환시킨다
	//--------------------------------------------------------------------------

	///image
	//--------------------------------------------------------------------------
	ID2D1Bitmap* LoadImageFile(const wchar_t* path);	//이미지 비트맵에 올리기
	void DrawBitmap(Graphics::Sprite* const sprite, const float opacity = 1.0f);	//비트맵 그리기
	void ImageRelease(ID2D1Bitmap*& image);	//이미지 저장되어있는 이미지 메모리 해제
	//--------------------------------------------------------------------------

	///이펙트
	//------------------------------------------------------------
	ID2D1Bitmap* GausBlur(Graphics::Sprite* const sprite, const float blurCoefficient);
	ID2D1Bitmap* Convolve(Graphics::Sprite* const sprite);
	ID2D1Bitmap* DirectionBlur(Graphics::Sprite* const sprite, const float blurCoefficient);
	ID2D1Bitmap* EdgeDetection(Graphics::Sprite* const sprite, const float lineThickness, const float blur);
	ID2D1Bitmap* Morphology(Graphics::Sprite* const sprite);
	ID2D1Bitmap* DisplacementMap(Graphics::Sprite* const sprite, const float effectPower);
	ID2D1Bitmap* Sepia(Graphics::Sprite* const sprite, const float coefficient);
	ID2D1Bitmap* PointDiffuse(Graphics::Sprite* const sprite, Graphics::BaseFigure::Point pos, const float& red, const float& green, const float& blue);
	ID2D1Bitmap* Posterize(Graphics::Sprite* const sprite);
	ID2D1Bitmap* Emboss(Graphics::Sprite* const sprite);
	ID2D1Bitmap* Shadow(Graphics::Sprite* const sprite);

	//사용권장 안함 - 나도 이해 안댐
	//---------------------------------------------------------------------
	ID2D1Bitmap* DistantDiffuse(Graphics::Sprite* const sprite, const float& red, const float& green, const float& blue);
	ID2D1Bitmap* DistantSpecular(Graphics::Sprite* const sprite, const float& red, const float& green, const float& blue);
	//---------------------------------------------------------------------

	/// 이미지 연출
	void FadeInFadeOut(Graphics::Sprite* image1, Graphics::Sprite* image2, D2D1_VECTOR_4F coefficient);

	// 클리핑
	void StartClipping(int left, int top, int right, int bottom);
	void EndClipping();

	///이미지 합치기
	ID2D1Bitmap* ArithmeticComposite(Graphics::Sprite* image1, Graphics::Sprite* image2, D2D1_VECTOR_4F coefficient);
	ID2D1Bitmap* ArithmeticComposite(Graphics::Sprite* image1, Graphics::Sprite* image2);
	ID2D1Bitmap* ArithmeticCompositeBitmap(ID2D1Bitmap* image1, ID2D1Bitmap* image2, UINT32 width, UINT32 height);
	ID2D1Bitmap* Blend(Graphics::Sprite* image1, Graphics::Sprite* image2);
	//------------------------------------------------------------

	///text파일 읽고 저장하는 함수
	//------------------------------------------------------------
	//모션에 저장
	void ReadSpriteTxtFile(const std::string textFileName, ID2D1Bitmap* image, Graphics::Motion* motion);
	//스프라이트만 저장
	Graphics::Sprite* ReadSpriteTxtFile(const std::string textFileName, ID2D1Bitmap* image);
	//------------------------------------------------------------

	//matrix와 vector2 곱셈
	static MathEngine::Vector2 MarixToVector(D2D1_MATRIX_3X2_F& matrix, MathEngine::Vector2 point)
	{
		MathEngine::Vector2 temp;
		temp.x = matrix.m11 * point.x + matrix.m12 * point.y + matrix.dx;
		temp.y = matrix.m21 * point.x + matrix.m22 * point.y + matrix.dy;

		return temp;
	}

	
	/// 파티클
	//test
	bool ispaticle = false;
	//------------------------------------------------------------
	//파티클을 만들고 셋팅한다
	void SetPaticle(MathEngine::Vector2 pos, float deadTime, int movementSeed, Graphics::Sprite* sprite);
	
	//파티클을 그린다
	void PaticleEffect(D2D1_MATRIX_3X2_F matrix);
	void PaticeUpdate(float deltatTime);

	void DeletePaticleEffect(); 
	//------------------------------------------------------------

private:
	//초기화 함수
	int Initialize();
	//글자 정렬
	void SetTextAlignment(TextAlignMent alignment);
	//문자 출력을 위한 기본 형식지정
	void SetTextFormat(const float fontsize = 20, const WCHAR* msc_fontName = L"Danjo-bold");
	//내부적으로 편의상 사용한 함수
	void FillRectangle(float leftTopX, float leftTopY, float rightBottomX, float rightBottomY, float thickness = 1.0f);

	//image를 bitmap으로 바꾼다
	ID2D1Bitmap* ImageToBitmap(ID2D1Image* pImg, Graphics::Sprite* const sprite);
	ID2D1Bitmap* ImageToBitmap(ID2D1Image* pImg, ID2D1Bitmap* image, UINT32 width, UINT32 height);

	//글자 정렬 설정
private:
	//윈도 핸들
	HWND m_hWnd;

	//클라이언트영역 사이즈
	RECT m_clientSize;

	//그리기 대상(렌더 타겟)
	ID2D1HwndRenderTarget* m_renderTarget;

	//Factory
	ID2D1Factory* m_d2dFactory;			//D2D
	IDWriteFactory* m_dWriteFactory;	//DWrite
	IDWriteTextFormat* m_textFormat;	//DWrite
	IWICImagingFactory* m_wicImageFactory;	//WIC

	//편의성
	IDWriteTextLayout* m_textLayOut;	//drawtext
	PaticleMaintain* m_paticleManager;

	//그리기 도구
	ID2D1SolidColorBrush* m_tempBrush;
	ID2D1SolidColorBrush* m_mainBrush;

	//스프라이트 애니메이션
	IWICBitmapDecoder* m_decoder;	//압축된 이미지를 해제할 객체
	IWICBitmapFrameDecode* m_frameDecode;	//특정 그림을 선택한 객체
	IWICFormatConverter* m_converter;	//이미지 변환 객체

	/// 이펙트 https://learn.microsoft.com/ko-kr/windows/win32/direct2d/built-in-effects
	//------------------------------------------------------------------------------------------------
	ID2D1DeviceContext* m_deviceContext;	//이펙트 사용을 위한 devicecontext (rendertarget의 상위)

	ID2D1Image* m_afterImage;	//이펙트를 적용한 이미지 저장 용도
	ID2D1Bitmap* m_afterBitmap;	//이펙트를 적용한 이미지를 비트맵으로 변환해 저장할 용도

	//필터 effect
	ID2D1Effect* m_gaussianBlur;	//가우스 블러
	ID2D1Effect* m_convolve;	//convolve 행렬 효과
	ID2D1Effect* m_directionBlur;	//방향 블러
	ID2D1Effect* m_edgeDetection;	//윤곽선 검출
	ID2D1Effect* m_morphology;	//형태

	//원본 효과
	ID2D1Effect* m_turbulence;	//난기류
	ID2D1Effect* m_flood;	//홍수? 발번역

	//조명 및 스타일화
	ID2D1Effect* m_displacementMap;	//레이어 왜곡
	ID2D1Effect* m_distantDiffuse;	//먼 확산 조명
	ID2D1Effect* m_distantSpecular;	//먼 반사 조명
	ID2D1Effect* m_emboss;	//이미지를 종이처럼 회색조
	ID2D1Effect* m_pointDiffuse;	//점 확산
	ID2D1Effect* m_posterize;	//포스터
	ID2D1Effect* m_shadow;	//그림자

	//변환
	ID2D1Effect* m_2DaffineTransform;	//2d 아핀 변환

	//구성
	ID2D1Effect* m_composite;	//복합 효과 - 2개 이상의 이미지 결합 걍 합치기
	ID2D1Effect* m_arithmeticComposite;	//산술 복합 효과 - 이미지 가중치로 결합
	ID2D1Effect* m_blend;	//혼합

	//사진
	ID2D1Effect* m_sepia;	//세피아톤으로 변환
	//------------------------------------------------------------------------------------------------

	ID2D1Bitmap* m_unFound;	//생성자에서 경로에 맞게 넣어주기
};