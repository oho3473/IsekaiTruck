// DirectY.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//
//

#include "DirectY.h"
#include "Sprite.h"
#include "PaticleMaintain.h"

#include <string>	//DrawText() 내부 _vscprintf를 사용하기 위한 헤더

#include<iostream>
#include <fstream>
#include <cstddef>        // std::size_t

/// <summary>
/// 생성자
/// 생성과 동시에 내부적으로 초기화까지 했다
/// </summary>
/// <param name="hWnd">윈도우 핸들</param>
DirectY::DirectY(HWND hWnd) : m_hWnd(hWnd), m_clientSize{NULL}, m_renderTarget(NULL), m_d2dFactory(NULL),
m_dWriteFactory(NULL), m_textFormat(NULL), m_wicImageFactory(NULL), m_textLayOut(NULL),m_paticleManager(NULL), m_tempBrush(NULL), m_mainBrush(NULL),
m_decoder(NULL), m_frameDecode(NULL), m_converter(NULL), m_deviceContext(NULL), m_afterImage(NULL), m_afterBitmap(NULL),
m_gaussianBlur(NULL), m_convolve(NULL), m_directionBlur(NULL), m_edgeDetection(NULL), m_displacementMap(NULL),
m_turbulence(NULL), m_distantDiffuse(NULL), m_distantSpecular(NULL), m_emboss(NULL), m_pointDiffuse(NULL), m_posterize(NULL),
m_shadow(NULL), m_2DaffineTransform(NULL), m_composite(NULL), m_arithmeticComposite(NULL), m_sepia(NULL)
{
	Initialize();
	//test 실제 게임에서는 경로 바꿔줘야한다
}

/// <summary>
/// 소멸자
/// </summary>
DirectY::~DirectY()
{
	delete m_paticleManager;

	if (m_converter != NULL)
	{
		m_converter->Release();
	}

	if (m_frameDecode != NULL)
	{
		m_frameDecode->Release();
	}

	//나중에 주석풀고 쓸 예정
	//m_unFound->Release();

	m_mainBrush->Release();
	m_tempBrush->Release();

	m_gaussianBlur->Release();
	m_convolve->Release();
	m_directionBlur->Release();
	m_edgeDetection->Release();
	m_displacementMap->Release();
	m_turbulence->Release();
	m_distantDiffuse->Release();
	m_distantSpecular->Release();
	m_emboss->Release();
	m_pointDiffuse->Release();
	m_posterize->Release();
	m_shadow->Release();
	m_2DaffineTransform->Release();
	m_composite->Release();
	m_arithmeticComposite->Release();
	m_sepia->Release();

	m_deviceContext->Release();

	m_wicImageFactory->Release();
	m_textFormat->Release();
	m_dWriteFactory->Release();
	m_d2dFactory->Release();

	// font 해제
	const std::wstring& path = L"Danjo-bold-Regular.otf";
	RemoveFontResource(path.c_str());
	::SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);
}

/// <summary>
/// 그리기 위해 해야하는 선행 함수
/// </summary>
void DirectY::BeginDraw()
{
	m_renderTarget->BeginDraw();
}

/// <summary>
/// 그리기가 끝나면 해주는 마무리 함수
/// </summary>
void DirectY::EndDraw()
{
	m_renderTarget->EndDraw();
}

/// <summary>
/// 검은색으로 전체를 채워 화면을 지움
/// </summary>
void DirectY::CleanWindow()
{
	SetBrushColor((float)102 / 255, (float)102 / 255, 1.f, 1.f);
	//SetBrushColor(1, 0.4, 1, 1);
	FillRectangle((float)m_clientSize.left, (float)m_clientSize.top, (float)m_clientSize.right, (float)m_clientSize.bottom);
}

/// <summary>
/// 브러쉬의 색상을 지정해준다
/// </summary>
/// <param name="red"></param>
/// <param name="green"></param>
/// <param name="blue"></param>
/// <param name="alpha"></param>
void DirectY::SetBrushColor(const float& red, const float& green, const float& blue, const float& alpha)
{
	m_tempBrush->Release();
	m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(red, green, blue, alpha)), &m_tempBrush);
}

/// <summary>
/// 브러쉬의 색상을 지정해준다
/// </summary>
/// <param name="red"></param>
/// <param name="green"></param>
/// <param name="blue"></param>
/// <param name="alpha"></param>
void DirectY::SetBrushColor(const int& red, const int& green, const int& blue, const int& alpha)
{
	m_tempBrush->Release();
	m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF((float)red, (float)green, (float)blue, (float)alpha)), &m_tempBrush);
}

/// <summary>
/// 브러쉬의 색상을 지정해준다
/// </summary>
/// <param name="color">{r,g,b,a}</param>
void DirectY::SetBrushColor(D2D1::ColorF color)
{
	m_tempBrush->Release();
	m_renderTarget->CreateSolidColorBrush(color, &m_tempBrush);
}

/// <summary>
/// 지정한 좌표 2개를 잇는 선을 그린다
/// </summary>
/// /// <param name="line">구조체</param>
/// <param name="thickness">두께 기본 값 = 5.0f</param>
void DirectY::DrawLine(Graphics::BaseFigure::Line line, const float thickness /*= 5.0f */)
{
	m_renderTarget->DrawLine(D2D1::Point2F(line.startX, line.startY), D2D1::Point2F(line.endX, line.endY), m_tempBrush, thickness);
}

/// <summary>
/// 속이 빈 사각형 그리기
/// </summary>
/// /// <param name="rect">구조체</param>
/// <param name="thickness">두께 기본 값 = 1.0f</param>
void DirectY::DrawRectangle(Graphics::BaseFigure::Rectangle rect, const float thickness /*= 1.0f*/)
{
	m_renderTarget->DrawRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), m_tempBrush, thickness);
}

/// <summary>
/// 원 그리기
/// </summary>
/// <param name="circle">구조체</param>
/// <param name="thickness">두께 기본 값 = 1.0f</param>
void DirectY::DrawCircle(Graphics::BaseFigure::Circle circle, const float thickness /*= 1.0f*/)
{
	D2D1_ELLIPSE D2Dcircle = { D2D1::Point2F(circle.centerX, circle.centerY), circle.radius,circle.radius };
	m_renderTarget->DrawEllipse(D2Dcircle, m_tempBrush, thickness);
}

/// <summary>
/// 타원 그리기
/// </summary>
/// <param name="ellipse">구조체</param>
/// <param name="thickness">두께 기본 값 = 1.0f</param>
void DirectY::DrawEllipse(Graphics::BaseFigure::Ellipse ellipse, const float thickness /*= 1.0f*/)
{
	D2D1_ELLIPSE D2Dellipse = { D2D1::Point2F(ellipse.centerX,ellipse.centerY), ellipse.radiusX,ellipse.radiusY };
	m_renderTarget->DrawEllipse(D2Dellipse, m_tempBrush, thickness);
}

/// <summary>
/// 속이 찬 사각형 그리기
/// </summary>
/// <param name="rect">구조체</param>
/// <param name="thickness">두께 기본 값 = 1.0f</param>
void DirectY::FillRectangle(Graphics::BaseFigure::Rectangle rect, const float thickness /*= 1.0f*/)
{
	m_renderTarget->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), m_tempBrush);
}

/// <summary>
/// 이세계 용사 전용 아소매트릭 그리기
/// </summary>
/// <param name="tile"></param>
/// <param name="thickness"></param>
void DirectY::DrawIsometric(Graphics::BaseFigure::IsomatricTile tile, const float thickness /*= 0.5f*/)
{
	DrawLine({ tile.centerXY.x - tile.width,tile.centerXY.y,tile.centerXY.x ,tile.centerXY.y + tile.height }, thickness);
	DrawLine({ tile.centerXY.x ,tile.centerXY.y + tile.height, tile.centerXY.x + tile.width,tile.centerXY.y }, thickness);
	DrawLine({ tile.centerXY.x + tile.width,tile.centerXY.y,tile.centerXY.x ,tile.centerXY.y - tile.height }, thickness);
	DrawLine({ tile.centerXY.x ,tile.centerXY.y - tile.height,tile.centerXY.x - tile.width,tile.centerXY.y }, thickness);
}


/// <summary>
/// 글자 그리기
/// </summary>
/// <param name="command">출력 내용</param>
/// <param name="x">글자를 그릴 사각형 left</param>
/// <param name="y">글자를 그릴 사각형 top</param>
/// <param name="fontsize">폰트크기</param>
/// <param name="msc_fontName">폰트 이름</param>
void DirectY::DrawText(const WCHAR* command, const float& x, const float& y, const float& fontsize, TextAlignMent alignment/*TextAlignMent::LEFT*/,const WCHAR* msc_fontName /*= L"Verdana"*/ )
{
	//textlayout으로 정보를 받을 구조체
	DWRITE_TEXT_METRICS textRect;

	//폰트와 글자 크기 바꾸기
	SetTextFormat(fontsize, msc_fontName);
	SetTextAlignment(alignment);
	
	//글자의 길이 최대 값 지정
	m_dWriteFactory->CreateTextLayout(command,wcslen(command),m_textFormat, 1300,1000,&m_textLayOut);
	//모든 정보 받아오기
	m_textLayOut->GetMetrics(&textRect);

	if (alignment == TextAlignMent::RIGHT)
	{
		m_renderTarget->DrawTextW(command, (UINT32)wcslen(command), m_textFormat, D2D1_RECT_F{ x - textRect.width -10, y, x, y + textRect.height }, m_tempBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
	else
	{
		m_renderTarget->DrawTextW(command, (UINT32)wcslen(command), m_textFormat, D2D1_RECT_F{ x ,y,x + textRect.width + 10,y + textRect.height },m_tempBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	m_textLayOut->Release();
}

/// <summary>
/// 글자 그리기
/// %d 같은 C언어처럼 입력
/// </summary>
/// <param name="x">글자 X</param>
/// <param name="y">글자 Y</param>
/// <param name="fontsize">폰트 사이즈</param>
/// <param name="pch">출력 내용</param>
/// <param name="msc_fontName">폰트 이름</param>
/// <param name="">기타 %f,%d에 쓸 인자들</param>
/// <returns></returns>
bool DirectY::DrawText(const int& x, const int& y, const float& fontsize, TextAlignMent alignment, const WCHAR* msc_fontName, const char* pch, ...)
{
	SetTextFormat(fontsize, msc_fontName);
	SetTextAlignment(alignment);

	// 포맷 스트링 관련 버퍼, 
	char* buffer;
	int len;

	// 가변인자 처리
	va_list ap;			// 현재 읽고 있는 포인터를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	//문자열의 가변인수의 개수를 찾는
	len = _vscprintf(pch, ap) + 1;
	buffer = new char[len];

	vsprintf_s(buffer, len, pch, ap);
	va_end(ap);

	//int nLen = lstrlenA(pch);
	// lstrlen((LPCSTR)buffer) 원래는 이거 쓰는데 길이를 제대로 반환하지 못함 같은 char*으로 길이를 가져오는 strlen으로 대체했다
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);

	BSTR unicodestr = SysAllocStringLen(NULL, nLen);

	// 당연히 멀티바이트와 유니코드의 문자열 길이 차이가 있다. 2바이트 문자 등
	::MultiByteToWideChar(CP_ACP, 0, buffer, (int)strlen(buffer), unicodestr, nLen);

	if (unicodestr != NULL)
	{
		// 폰트별 차지하는 영역 계산 필요
		m_renderTarget->DrawText(unicodestr, (UINT32)wcslen(unicodestr),
			m_textFormat, D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 100)), m_tempBrush);
	}

	//... when done, free the BSTR
	::SysFreeString(unicodestr);

	// 포맷관련 버퍼 삭제
	delete[] buffer;

	return true;
}

DWRITE_TEXT_METRICS DirectY::GetTextMetrics(const WCHAR* command)
{
	//textlayout으로 정보를 받을 구조체
	DWRITE_TEXT_METRICS textRect;

	//폰트와 글자 크기 바꾸기

	//글자의 길이 최대 값 지정
	m_dWriteFactory->CreateTextLayout(command, wcslen(command), m_textFormat, 1000, 1000, &m_textLayOut);
	//모든 정보 받아오기
	m_textLayOut->GetMetrics(&textRect);

	m_textLayOut->Release();

	return textRect;
}

/// <summary>
/// 그림에 변환 행렬을 적용한다
/// </summary>
/// <param name="matrix">MakeMatrix의 TranslateMatrix()</param>
void DirectY::SetTransform(const D2D1_MATRIX_3X2_F& matrix)
{
	m_renderTarget->SetTransform(matrix);
}

/// <summary>
/// 경로에 있는 특정 파일을 비트맵에 올려준다
/// 초기화 할때 한번만 쓸 것
/// </summary>
/// <param name="path">경로</param>
/// <returns>bitmap(리소스 매니저에 등록할 것)</returns>
ID2D1Bitmap* DirectY::LoadImageFile(const wchar_t* path)
{
	//HRESULT a = m_wicImageFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &m_decoder);
	//HRESULT b = m_decoder->GetFrame(0, &m_frameDecode);
	//이미지 압출해제를 위한 객체를 생성

	ID2D1Bitmap* temp = NULL;

	if (S_OK == (m_wicImageFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &m_decoder)))
	{
		//파일 구성하는 이미지 중에서 첫번째 이미지를 선택
		if (S_OK == (m_decoder->GetFrame(0, &m_frameDecode)))
		{
			//wic 형식의 비트 맵이기때문에 바로 사용하기 어렵다,D2DBitmap으로 변환
			if (S_OK == m_wicImageFactory->CreateFormatConverter(&m_converter))
			{
				//선택된 그림을 어떤 형식의 비트맵으로 변환할 것인지 설정
				if (S_OK == m_converter->Initialize(m_frameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom))
				{
					//IWICBitmap 형식의 비트맵으로 ID2D1Bitmap 객체 생성
					if (S_OK == m_renderTarget->CreateBitmapFromWicBitmap(m_converter, NULL, &temp))
					{
						return temp;
					}
				}
			}
		}
	}
	else
	{
		MessageBox(m_hWnd, L"Image load Failed", path, MB_OK);
		m_wicImageFactory->CreateDecoderFromFilename(L"Resource/unfound.png", NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &m_decoder);

		m_decoder->GetFrame(0, &m_frameDecode);

		m_wicImageFactory->CreateFormatConverter(&m_converter);
		m_converter->Initialize(m_frameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);

		m_renderTarget->CreateBitmapFromWicBitmap(m_converter, NULL, &temp);

		return temp;
	}
	
	return temp;
}

/// <summary>
/// bitmap에 있는 이미지의 특정 위치를 그린다
/// 이걸 프레임 단위로 다른 위치의 이미지를 그려주면 스프라이트애니메이션
/// </summary>
/// <param name="sprite"></param>
/// <param name="opacity">투명도(0~1.0f) 기본1.0f</param>
void DirectY::DrawBitmap(Graphics::Sprite* const sprite, const float opacity /*= 1.0f*/)
{
	static bool showerror = false;
	//m_RenderTarget->DrawBitmap(m_BitMap,&size,명도=1,보간방법=0(선형보간),&내가 그릴 크기=0(전체));
	ID2D1Bitmap* temp = NULL;
	D2D1_RECT_F tempSize = {sprite->m_drawPos.x,sprite->m_drawPos.y,sprite->m_drawPos.x+sprite->m_drawSize.right,sprite->m_drawPos.y + sprite->m_drawSize.bottom};

	if (sprite->m_image != NULL)
	{
		temp = sprite->m_image;

		//SetImage(sprite->m_image);
		m_renderTarget->DrawBitmap(temp, tempSize, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sprite->m_drawArea);
	}
	else
	{
		//에러 스프라이트의 이름
		std::wstring WStr;
		WStr.assign(sprite->m_name.begin(), sprite->m_name.end());
		LPCWSTR errorfile = WStr.c_str();

		//temp에 unfound 이미지 셋팅이 필요함
		if (!showerror)
		{
			MessageBox(m_hWnd, L"Image Draw Failed", errorfile, MB_OK);
			showerror = true;
		}
		m_renderTarget->DrawBitmap(m_unFound, tempSize, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sprite->m_drawArea);
 	}
}

/// <summary>
/// 이미지를 해제한다
/// </summary>
void DirectY::ImageRelease(ID2D1Bitmap*& image)
{
	image->Release();
}

/// <summary>
/// 이미지를 가우스 블러 처리해 그린다
/// </summary>
/// <param name="sprite"></param>
/// <param name="blurCoefficient">블러의 강도</param>
ID2D1Bitmap* DirectY::GausBlur(Graphics::Sprite* const sprite, const float blurCoefficient)
{
	m_gaussianBlur->SetInput(0, sprite->m_image);
	m_gaussianBlur->SetValue(D2D1_BORDER_MODE_SOFT, blurCoefficient);

	m_gaussianBlur->GetOutput(&m_afterImage);
	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_gaussianBlur->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &m_gaussianBlur);

	return m_afterBitmap;
}

/// <summary>
/// convolve 행렬 효과를 처리해 그린다
/// </summary>
/// <param name="sprite"></param>
ID2D1Bitmap* DirectY::Convolve(Graphics::Sprite* const sprite)
{
	m_convolve->SetInput(0, sprite->m_image);

	float matrix[9] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };
	m_convolve->SetValue(D2D1_CONVOLVEMATRIX_PROP_KERNEL_MATRIX, matrix);
	m_convolve->GetOutput(&m_afterImage);
	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_convolve->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1ConvolveMatrix, &m_convolve);

	return m_afterBitmap;
}

/// <summary>
/// 이미지를 특정 방향으로 블러처리해 그린다
/// </summary>
/// <param name="sprite"></param>
/// <param name="blurCoefficient">블러의 강도</param>
ID2D1Bitmap* DirectY::DirectionBlur(Graphics::Sprite* const sprite, const float blurCoefficient)
{
	m_directionBlur->SetInput(0, sprite->m_image);
	m_directionBlur->SetValue(D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION, blurCoefficient);
	m_directionBlur->GetOutput(&m_afterImage);
	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_directionBlur->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1DirectionalBlur, &m_directionBlur);

	return m_afterBitmap;
}

/// <summary>
/// 이미지의 윤곽선을 검출해 그린다
/// </summary>
/// <param name="sprite"></param>
/// <param name="lineThickness">윤곽선의 강도</param>
/// <param name="blur">이미지 대비 강도</param>
ID2D1Bitmap* DirectY::EdgeDetection(Graphics::Sprite* const sprite, const float lineThickness, const float blur)
{
	
	m_edgeDetection->SetInput(0, sprite->m_image);
	m_edgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_STRENGTH, lineThickness);
	m_edgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_BLUR_RADIUS, blur);
	m_edgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_MODE, D2D1_EDGEDETECTION_MODE_SOBEL);
	m_edgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_OVERLAY_EDGES, false);
	m_edgeDetection->SetValue(D2D1_EDGEDETECTION_PROP_ALPHA_MODE, D2D1_ALPHA_MODE_PREMULTIPLIED);

	m_edgeDetection->GetOutput(&m_afterImage);
	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_edgeDetection->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1EdgeDetection, &m_directionBlur);

	return m_afterBitmap;
}

/// <summary>
/// 그림의 형태를 굵게 또는 얇게 그린다
/// </summary>
/// <param name="sprite"></param>
ID2D1Bitmap* DirectY::Morphology(Graphics::Sprite* const sprite)
{
	m_morphology->SetInput(0, sprite->m_image);
	m_morphology->SetValue(D2D1_MORPHOLOGY_PROP_MODE, D2D1_MORPHOLOGY_MODE_ERODE);
	m_morphology->SetValue(D2D1_MORPHOLOGY_PROP_WIDTH, 14);
	m_morphology->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_morphology->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1Morphology, &m_morphology);

	return m_afterBitmap;
}

/// <summary>
///  화면상 drawpoint 영역에 effect를 그리고 이미지를 그 영역에 맞게 그려 effect를 적용시킨다
/// drawPoint 영역보다 imageXY의 영역이 크면 안된다
/// 기존의 effect와는 다르게 적용 effect의 영역이 있다
/// </summary>
/// <param name="sprite"></param>
/// <param name="effectPower">effect의 적용 강도</param>
ID2D1Bitmap* DirectY::DisplacementMap(Graphics::Sprite* const sprite, const float effectPower)
{
	m_displacementMap->SetInput(0, sprite->m_image);
	m_displacementMap->SetValue(D2D1_DISPLACEMENTMAP_PROP_SCALE, effectPower);

	m_turbulence->SetValue(D2D1_TURBULENCE_PROP_OFFSET, D2D1_VECTOR_2F{ sprite->m_drawPos.x,sprite->m_drawPos.y });
	m_turbulence->SetValue(D2D1_TURBULENCE_PROP_SIZE, D2D1_VECTOR_2F{ sprite->m_drawPos.x + sprite->m_drawSize.left,sprite->m_drawPos.y + sprite->m_drawSize.top });

	m_displacementMap->SetInputEffect(1, m_turbulence);
	m_displacementMap->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_displacementMap->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1DisplacementMap, &m_displacementMap);

	return m_afterBitmap;
}

/// <summary>
/// sepia 톤으로 이미지 변환
/// 강도(0~1.0) 0에 가까울수록 회색
/// </summary>
/// <param name="sprite">이미지</param>
/// <param name="coefficient">세피아효과강도(0.0f~1.0f)</param>
ID2D1Bitmap* DirectY::Sepia(Graphics::Sprite* const sprite, const float coefficient)
{
	m_sepia->SetInput(0, sprite->m_image);
	m_sepia->SetValue(D2D1_SEPIA_PROP_INTENSITY, coefficient);
	m_sepia->SetValue(D2D1_SEPIA_PROP_ALPHA_MODE, D2D1_ALPHA_MODE_PREMULTIPLIED);

	m_sepia->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_sepia->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1Sepia, &m_sepia);
	return m_afterBitmap;
}

/// <summary>
/// 점 확산 - 방향지시등 혹은 반짝이 느낌으로 써도 좋을듯
/// 이미지 만들어서 셋팅해줘야한다
/// </summary>
/// <param name="sprite">전체 이미지</param>
/// <param name="pos">전체 이미지에서 어디에 점(광원)을 찍을지</param>
/// <param name="red"></param>
/// <param name="green"></param>
/// <param name="blue"></param>
/// <returns></returns>
ID2D1Bitmap* DirectY::PointDiffuse(Graphics::Sprite* const sprite, Graphics::BaseFigure::Point pos, const float& red, const float& green, const float& blue)
{
	m_pointDiffuse->SetInput(0, sprite->m_image);
	m_pointDiffuse->SetValue(D2D1_POINTDIFFUSE_PROP_LIGHT_POSITION, D2D1_VECTOR_3F{ pos.x,pos.y,2.f }); //점 광원 위치z값에 따라 빛이 보이냐 안보이냐
	m_pointDiffuse->SetValue(D2D1_POINTDIFFUSE_PROP_DIFFUSE_CONSTANT, 50.f); //광원의 양에 대한 확산 반사 비율
	m_pointDiffuse->SetValue(D2D1_POINTDIFFUSE_PROP_SURFACE_SCALE, 1.0f); //이미지의 z 배율 - 광원의 뚜렷함?
	m_pointDiffuse->SetValue(D2D1_POINTDIFFUSE_PROP_COLOR, D2D1_VECTOR_3F{ red,green,blue }); //색상

	m_pointDiffuse->GetOutput(&m_afterImage);
	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_pointDiffuse->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1PointDiffuse, &m_pointDiffuse);

	return m_afterBitmap;
}

/// <summary>
/// 포스터
/// </summary>
/// <param name="sprite"></param>
ID2D1Bitmap* DirectY::Posterize(Graphics::Sprite* const sprite)
{
	m_posterize->SetInput(0, sprite->m_image);
	m_posterize->SetValue(D2D1_POSTERIZE_PROP_RED_VALUE_COUNT, 4);
	m_posterize->SetValue(D2D1_POSTERIZE_PROP_GREEN_VALUE_COUNT, 4);
	m_posterize->SetValue(D2D1_POSTERIZE_PROP_BLUE_VALUE_COUNT, 4);

	m_posterize->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	return m_afterBitmap;
}

/// <summary>
/// 이미지 종이처럼 그려준다 - 회색조
/// </summary>
/// <param name="sprite"></param>
ID2D1Bitmap* DirectY::Emboss(Graphics::Sprite* const sprite)
{
	m_emboss->SetInput(0, sprite->m_image);
	m_emboss->SetValue(D2D1_EMBOSS_PROP_HEIGHT, 1.0f); //종이화 강도
	m_emboss->SetValue(D2D1_EMBOSS_PROP_DIRECTION, 0.0f); //광원 방향

	m_emboss->GetOutput(&m_afterImage);
	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_emboss->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1Emboss, &m_emboss);

	return m_afterBitmap;
}

/// <summary>
/// 원래 이미지를 검게 만들어 그림자 이미지를 만들뿐
/// 원래 이미지 그리기 전에 그려 그림자가 있는것 처럼해야된다
/// </summary>
/// <param name="image"></param>
/// <param name="drawPoint"></param>
/// <param name="imageXY"></param>
ID2D1Bitmap* DirectY::Shadow(Graphics::Sprite* const sprite)
{
	m_shadow->SetInput(0, sprite->m_image);
	m_shadow->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, 3.0f);	//그림자 흐림 기본값3.0f

	m_flood->SetValue(D2D1_FLOOD_PROP_COLOR, D2D1::Vector4F(1.0f, 0.0f, 1.0f, 0.0f));	//비트맵 색상 지정 - 뒷 배경 크로마키로 없애줘야한다

	m_2DaffineTransform->SetInputEffect(0, m_shadow);

	D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation(20, 20);
	m_2DaffineTransform->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, matrix);

	m_composite->SetInputEffect(0, m_flood);
	m_composite->SetInputEffect(1, m_2DaffineTransform);
	m_composite->SetInput(2, sprite->m_image);

	m_composite->GetOutput(&m_afterImage);
	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	return m_afterBitmap;
}

/// <summary>
///조명에따른 외곽선
/// 먼 거리에서 발생하는 확산광 - 빛의 방향
/// 그라데이션
/// <param name="sprite">이미지</param>
/// <param name="red">들어오는 광선 색R</param>
/// <param name="green">들어오는 광선 색G</param>
/// <param name="blue">들어오는 광선 색B</param>
ID2D1Bitmap* DirectY::DistantDiffuse(Graphics::Sprite* const sprite, const float& red, const float& green, const float& blue)
{
	m_distantDiffuse->SetInput(0, sprite->m_image);
	m_distantDiffuse->SetValue(D2D1_DISTANTDIFFUSE_PROP_AZIMUTH, 0.f);	//시계 반대 방향의 X축을 기준으로 XY 평면의 광원 방향 각도
	m_distantDiffuse->SetValue(D2D1_DISTANTDIFFUSE_PROP_ELEVATION, 0.f);	//시계 반대 방향의 Y축을 기준으로 하는 YZ 평면의 광원 방향 각도
	m_distantDiffuse->SetValue(D2D1_DISTANTDIFFUSE_PROP_DIFFUSE_CONSTANT, 1.f);	//들어오는 광원의 양에 대한 확산 반사의 비율
	m_distantDiffuse->SetValue(D2D1_DISTANTDIFFUSE_PROP_SURFACE_SCALE, 5.f);	//Z 방향의 배율 인수
	m_distantDiffuse->SetValue(D2D1_DISTANTDIFFUSE_PROP_COLOR, D2D1_VECTOR_3F{ red,green,blue });	//들어오는 조명의 색

	m_distantDiffuse->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_distantDiffuse->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1DistantDiffuse, &m_distantDiffuse);

	return m_afterBitmap;
}
/// <summary>
/// 먼 거리에서 발생하는 반사광 -눈뽕
/// 반짝반짝
/// </summary>
/// <param name="sprite">이미지</param>
/// <param name="red"></param>
/// <param name="green"></param>
/// <param name="blue"></param>
ID2D1Bitmap* DirectY::DistantSpecular(Graphics::Sprite* const sprite, const float& red, const float& green, const float& blue)
{
	m_distantSpecular->SetInput(0, sprite->m_image);
	m_distantSpecular->SetValue(D2D1_DISTANTSPECULAR_PROP_AZIMUTH, 0.f);	//x축 기준 xy 평면의 각도
	m_distantSpecular->SetValue(D2D1_DISTANTSPECULAR_PROP_ELEVATION, 0.f);	//y축 기준 yz 평면의 각도
	m_distantSpecular->SetValue(D2D1_DISTANTSPECULAR_PROP_SPECULAR_EXPONENT, 1000.f);	//퐁 조명에 대한 이미지 반사율
	m_distantSpecular->SetValue(D2D1_DISTANTSPECULAR_PROP_SPECULAR_CONSTANT, 10000.0f);	//이미지 반사율 (0~10000) 낮을 수록 반사 안함
	m_distantSpecular->SetValue(D2D1_DISTANTSPECULAR_PROP_SURFACE_SCALE, 1.f);	//z 방향 배율
	m_distantSpecular->SetValue(D2D1_DISTANTSPECULAR_PROP_COLOR, D2D1_VECTOR_3F{ red,green,blue });	//외곽선 색

	m_distantSpecular->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, sprite);

	m_distantSpecular->Release();
	m_deviceContext->CreateEffect(CLSID_D2D1DistantSpecular, &m_distantSpecular);

	return m_afterBitmap;
}

/// <summary>
/// 이미지 교차 그리기
/// </summary>
/// <param name="image1">이미지1</param>
/// <param name="image2">이미지2</param>
/// <param name="coefficient">z값 안씀</param>
void DirectY::FadeInFadeOut(Graphics::Sprite* image1, Graphics::Sprite* image2, D2D1_VECTOR_4F coefficient)
{
	m_arithmeticComposite->SetInput(0, image1->m_image);
	m_arithmeticComposite->SetInput(1, image2->m_image);
	//출력rgba = C1 * Sourcergba * Destinationrgba + C2 * Sourcergba + C3 * Destinationrgba + C4
	//C1합성된 이미지의 투명도 C2 image 투명도 C3 image2의 투명도 C4 1에 가까울 수록 전체 이미지 하얀색
	m_arithmeticComposite->SetValue(D2D1_ARITHMETICCOMPOSITE_PROP_COEFFICIENTS, D2D1::Vector4F(coefficient.x, coefficient.y, 1 - coefficient.y, coefficient.w));

	m_deviceContext->DrawImage(m_arithmeticComposite);
}

/// <summary>
/// 클리핑 시작
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
void DirectY::StartClipping(int left, int top, int right, int bottom)
{
	D2D1_RECT_F clipRect = D2D1::RectF((FLOAT)left, (FLOAT)top, (FLOAT)right, (FLOAT)bottom);
	m_renderTarget->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_ALIASED);
}

/// <summary>
/// 클리핑 끝
/// </summary>
void DirectY::EndClipping()
{
	m_renderTarget->PopAxisAlignedClip();
}

/// <summary>
/// image1 베이스에 image2를 섞음
/// 이거를 이용해 조명을 만들 수 있다 - alpha 값으로 적절히 섞으면
/// </summary>
/// <param name="image1"></param>
/// <param name="image2"></param>
/// <param name="coefficient"></param>
/// <returns></returns>
ID2D1Bitmap* DirectY::ArithmeticComposite(Graphics::Sprite* image1, Graphics::Sprite* image2, D2D1_VECTOR_4F coefficient)
{
	m_arithmeticComposite->SetInput(0, image1->m_image);
	m_arithmeticComposite->SetInput(1, image2->m_image);
	//출력rgba = C1 * Sourcergba * Destinationrgba + C2 * Sourcergba + C3 * Destinationrgba + C4
	//C1합성된 이미지의 투명도 C2 image 투명도 C3 image2의 투명도 C4 1에 가까울 수록 전체 이미지 하얀색
	m_arithmeticComposite->SetValue(D2D1_ARITHMETICCOMPOSITE_PROP_COEFFICIENTS, D2D1::Vector4F(coefficient.x, coefficient.y, coefficient.z, coefficient.w));

	m_arithmeticComposite->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, image1);

	return m_afterBitmap;
}

ID2D1Bitmap* DirectY::ArithmeticComposite(Graphics::Sprite* image1, Graphics::Sprite* image2)
{
	m_arithmeticComposite->SetInput(0, image1->m_image);
	m_arithmeticComposite->SetInput(1, image2->m_image);
	//출력rgba = C1 * Sourcergba * Destinationrgba + C2 * Sourcergba + C3 * Destinationrgba + C4
	//C1합성된 이미지의 투명도 C2 image 투명도 C3 image2의 투명도 C4 1에 가까울 수록 전체 이미지 하얀색
	m_arithmeticComposite->SetValue(D2D1_ARITHMETICCOMPOSITE_PROP_COEFFICIENTS, D2D1::Vector4F(1, 1, 1, 0));

	m_arithmeticComposite->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, image2);

	return m_afterBitmap;
}

ID2D1Bitmap* DirectY::ArithmeticCompositeBitmap(ID2D1Bitmap* image1, ID2D1Bitmap* image2, UINT32 width, UINT32 height)
{
	m_arithmeticComposite->SetInput(0, image1);
	m_arithmeticComposite->SetInput(1, image2);
	//출력rgba = C1 * Sourcergba * Destinationrgba + C2 * Sourcergba + C3 * Destinationrgba + C4
	//C1합성된 이미지의 투명도 C2 image 투명도 C3 image2의 투명도 C4 1에 가까울 수록 전체 이미지 하얀색
	m_arithmeticComposite->SetValue(D2D1_ARITHMETICCOMPOSITE_PROP_COEFFICIENTS, D2D1::Vector4F(1, 1, 1, 0));

	m_arithmeticComposite->GetOutput(&m_afterImage);

	m_afterBitmap = ImageToBitmap(m_afterImage, image2,width,height);

	return m_afterBitmap;
}

/// <summary>
/// image1 위에 image2를 덮어 섞은 비트맵을 리턴
/// </summary>
/// <param name="image1"></param>
/// <param name="image2"></param>
/// <returns></returns>
ID2D1Bitmap* DirectY::Blend(Graphics::Sprite* image1, Graphics::Sprite* image2)
{
	m_blend->SetInput(0, image1->m_image);
	m_blend->SetInput(1, image2->m_image);
	//D2D1_BLEND_MODE_EXCLUSION - 다른거 쓰면 섞는 방식 바뀜
	m_blend->SetValue(D2D1_BLEND_PROP_MODE, D2D1_BLEND_MODE_EXCLUSION);

	m_blend->GetOutput(&m_afterImage);
	m_afterBitmap = ImageToBitmap(m_afterImage, image1);


	return m_afterBitmap;
}

/// <summary>
/// text 파일에서 정보를 읽어 저장하는 함수
/// </summary>
/// <param name="textFileName">text 파일 이름</param>
/// <param name="image">그릴 이미지 시트(비트맵)</param>
/// <param name="motion">정보를 관리할 Motion 객체</param>
void DirectY::ReadSpriteTxtFile(const std::string textFileName, ID2D1Bitmap* image, Graphics::Motion* motion)
{
	//text 형식
	//개수
	//이름, left, top, right, bottom, px, py,

	//읽어드리는 경로는, 비주얼 스튜디오 상에서 실행하였을 때 소스파일이 들어있는 위치와 동일해야한다(현재는 Demo위치)
	std::ifstream animationTextFile("Resource/Text/" + textFileName + ".txt");

	//text파일에서 읽는 1줄을 저장할 변수
	std::string data;

	int index = 0;
	std::size_t prefound;

	//txt 파일에서 받은 데이터 저장 용도
	std::string name = "";
	D2D1_RECT_F drawArea = {};
	D2D1_POINT_2F drawPos = {};
	D2D1_POINT_2F pivot = {};

	if (animationTextFile.is_open())
	{
		std::getline(animationTextFile, data);	//텍스트 파일 첫번째 줄은 전체 프레임 수
		motion->SetTotalFrame(stoi(data));	//전체 프레임 수 저장 및 벡터 크기 재지정

		// txt 파일 한 줄 씩 읽기
		while (std::getline(animationTextFile, data))
		{
			//한줄씩 읽어오면서 motion 내부 vector에 넣어 줘야한다

			// 받아온 string이(Line이) 공백일 경우 프로그램 종료
			if (data._Equal(""))
			{
				break;
			}

			// ','로 데이터를 구분하기에 해당 위치를 알아야한다
			std::size_t found = data.find_first_of(',');
			prefound = found;

			///name
			name = data.substr(0, found);

			///drawArea
			//-------------------------------------------------------------------------------------------
			found = data.find_first_of(',', found + 1);	//다음 ','를 찾는다
			drawArea.left = stof(data.substr(prefound + 1, found - prefound - 1));	//시작 위치부터 몇개를 사용할지
			prefound = found;	//이전 위치 저장

			found = data.find_first_of(',', found + 1);
			drawArea.top = stof(data.substr(prefound + 1, found - prefound - 1));
			prefound = found;

			found = data.find_first_of(',', found + 1);
			drawArea.right = stof(data.substr(prefound + 1, found - prefound - 1));
			prefound = found;

			found = data.find_first_of(',', found + 1);
			drawArea.bottom = stof(data.substr(prefound + 1, found - prefound - 1));
			prefound = found;
			//-------------------------------------------------------------------------------------------

			///pivot
			float width = drawArea.right - drawArea.left;
			float hegiht = drawArea.bottom - drawArea.top;
			//-------------------------------------------------------------------------------------------
			found = data.find_first_of(',', found + 1);
			pivot.x = width * stof(data.substr(prefound + 1, found - prefound - 1));
			prefound = found;

			found = data.find_first_of(',', found + 1);
			pivot.y = hegiht * stof(data.substr(prefound + 1, found - prefound - 1));
			prefound = found;
			//-------------------------------------------------------------------------------------------

			//sprite 저장
			//화면상 얼만큼으로 그릴건지
			motion->AddSprite(new Graphics::Sprite(name, index, 0, image, {0,0,width,hegiht }, drawArea, drawPos, pivot));

			index++;
		}
		animationTextFile.close();
	}
	else
	{
		std::wstring stemp = std::wstring(textFileName.begin(), textFileName.end());
		LPCWSTR sw = stemp.c_str();

		MessageBox(m_hWnd, L"File Open Failed", sw, MB_OK);
	}
	return;
}

Graphics::Sprite* DirectY::ReadSpriteTxtFile(const std::string textFileName, ID2D1Bitmap* image)
{
	//text 형식
	//개수
	//이름, left, top, right, bottom, px, py,

	//읽어드리는 경로는, 비주얼 스튜디오 상에서 실행하였을 때 소스파일이 들어있는 위치와 동일해야한다(현재는 Demo위치)
	std::ifstream animationTextFile("Resource/Text/" + textFileName + ".txt");

	//text파일에서 읽는 1줄을 저장할 변수
	std::string data;

	int index = 0;
	std::size_t prefound;

	//txt 파일에서 받은 데이터 저장 용도
	std::string name = "";
	D2D1_RECT_F drawArea = {};
	D2D1_POINT_2F drawPos = {};
	D2D1_POINT_2F pivot = {};

	if (animationTextFile.is_open())
	{
		std::getline(animationTextFile, data);	//텍스트 파일 첫번째 줄은 전체 프레임 수는 건너뛰기위해 한줄 읽기

		// txt 파일 한 줄 씩 읽기
		std::getline(animationTextFile, data);

		// ','로 데이터를 구분하기에 해당 위치를 알아야한다
		std::size_t found = data.find_first_of(',');
		prefound = found;

		///name
		name = data.substr(0, found);

		///drawArea
		//-------------------------------------------------------------------------------------------
		found = data.find_first_of(',', found + 1);	//다음 ','를 찾는다
		drawArea.left = stof(data.substr(prefound + 1, found - prefound - 1));	//시작 위치부터 몇개를 사용할지
		prefound = found;	//이전 위치 저장

		found = data.find_first_of(',', found + 1);
		drawArea.top = stof(data.substr(prefound + 1, found - prefound - 1));
		prefound = found;

		found = data.find_first_of(',', found + 1);
		drawArea.right = stof(data.substr(prefound + 1, found - prefound - 1));
		prefound = found;

		found = data.find_first_of(',', found + 1);
		drawArea.bottom = stof(data.substr(prefound + 1, found - prefound - 1));
		prefound = found;
		//-------------------------------------------------------------------------------------------

		///pivot
		float width = drawArea.right - drawArea.left;
		float hegiht = drawArea.bottom - drawArea.top;
		//-------------------------------------------------------------------------------------------
		found = data.find_first_of(',', found + 1);
		pivot.x = width * stof(data.substr(prefound + 1, found - prefound - 1));
		prefound = found;

		found = data.find_first_of(',', found + 1);
		pivot.y = hegiht * stof(data.substr(prefound + 1, found - prefound - 1));
		prefound = found;
		//-------------------------------------------------------------------------------------------

		animationTextFile.close();
		
		//sprite 저장
		//화면상 얼만큼으로 그릴건지

		Graphics::Sprite* temp = new Graphics::Sprite(name, index, 0, image, { 0,0,width,hegiht }, drawArea, drawPos, pivot);
		return temp;
	}
	else
	{
		std::wstring stemp = std::wstring(textFileName.begin(), textFileName.end());
		LPCWSTR sw = stemp.c_str();

		MessageBox(m_hWnd, L"File Open Failed", sw, MB_OK);
		return nullptr;
	}
}

void DirectY::SetPaticle(MathEngine::Vector2 pos, float deadTime, int movementSeed, Graphics::Sprite* sprite)
{
	m_paticleManager->CreatePaticle(pos, deadTime, movementSeed, sprite);
}

void DirectY::PaticleEffect(D2D1_MATRIX_3X2_F matrix)
{
	m_paticleManager->Render(this, matrix);
}

void DirectY::PaticeUpdate(float deltatTime)
{
	m_paticleManager->Update(deltatTime);
}

void DirectY::DeletePaticleEffect()
{
	m_paticleManager->DeletePaticle();
}

/// <summary>
/// 그래픽스 엔진 초기화
/// </summary>
/// <returns>실패0  성공1</returns>
int DirectY::Initialize()
{
	//팩토리 생성
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory);
	if (!SUCCEEDED(hr))
	{
		MessageBox(m_hWnd, L"D2DFactory Created Failed", NULL, MB_OK);
		return FALSE;
	}

	GetClientRect(m_hWnd, &m_clientSize);

	//렌더 타겟 생성
	hr = m_d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hWnd
			, D2D1::SizeU(m_clientSize.right - m_clientSize.left, m_clientSize.bottom - m_clientSize.top))
		, &m_renderTarget);

	//브러쉬 생성
	if (SUCCEEDED(hr))
	{
		m_renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&m_mainBrush);

		m_renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&m_tempBrush);
	}
	else
	{
		MessageBox(m_hWnd, L"D2DRenderTarget Created Failed", NULL, MB_OK);
		return FALSE;
	}

	const std::wstring& path = L"Danjo-bold-Regular.otf";
	// 폰트를 시스템에 추가
	int a = AddFontResource(path.c_str());
	int b = ::SendMessage(HWND_BROADCAST, WM_FONTCHANGE, NULL, NULL);

	//textformat
	static const WCHAR msc_fontName[] = L"Danjo";
	static const FLOAT msc_fontSize = 10;

	// Create a DirectWrite factory.
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dWriteFactory),
		reinterpret_cast<IUnknown**>(&m_dWriteFactory));

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_dWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&m_textFormat
		);
	}
	else
	{
		MessageBox(m_hWnd, L"DWriteFactory Created Failed", NULL, MB_OK);
		return FALSE;
	}

	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	else
	{
		MessageBox(m_hWnd, L"DWriteFormat Created Failed", NULL, MB_OK);
		return FALSE;
	}

	//초기화 없으면 create 안댐
	//현재 스레드에서 COM 라이브러리를 초기화하고 동시성 모델을 STA(단일 스레드 아파트)로 식별합니다.
	hr = CoInitialize(NULL);

	if (SUCCEEDED(hr))
	{
		//wicfactory create
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&m_wicImageFactory));
	}
	else
	{
		MessageBox(m_hWnd, L"WICFactory Created Failed", NULL, MB_OK);
		return FALSE;
	}

	//devicecontext 만들기
	hr = m_renderTarget->QueryInterface(&m_deviceContext);
	if (SUCCEEDED(hr))
	{
		//가우스블러
		m_deviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &m_gaussianBlur);

		//나선형 매트릭스
		m_deviceContext->CreateEffect(CLSID_D2D1ConvolveMatrix, &m_convolve);

		//특정 방향으로 블러
		m_deviceContext->CreateEffect(CLSID_D2D1DirectionalBlur, &m_directionBlur);

		//윤곽선 블러
		m_deviceContext->CreateEffect(CLSID_D2D1EdgeDetection, &m_edgeDetection);

		//형태
		m_deviceContext->CreateEffect(CLSID_D2D1Morphology, &m_morphology);

		//변위 맵
		m_deviceContext->CreateEffect(CLSID_D2D1DisplacementMap, &m_displacementMap);

		//난기류
		m_deviceContext->CreateEffect(CLSID_D2D1Turbulence, &m_turbulence);

		//먼 확산 조명
		m_deviceContext->CreateEffect(CLSID_D2D1DistantDiffuse, &m_distantDiffuse);

		//원격 반사 조명
		m_deviceContext->CreateEffect(CLSID_D2D1DistantDiffuse, &m_distantSpecular);

		//회색조
		m_deviceContext->CreateEffect(CLSID_D2D1Emboss, &m_emboss);

		//점 확산
		m_deviceContext->CreateEffect(CLSID_D2D1PointDiffuse, &m_pointDiffuse);

		//포스터
		m_deviceContext->CreateEffect(CLSID_D2D1Posterize, &m_posterize);

		//그림자
		m_deviceContext->CreateEffect(CLSID_D2D1Shadow, &m_shadow);

		//홍수
		m_deviceContext->CreateEffect(CLSID_D2D1Flood, &m_flood);

		//2d 아핀변환
		m_deviceContext->CreateEffect(CLSID_D2D12DAffineTransform, &m_2DaffineTransform);

		//복합 효과
		m_deviceContext->CreateEffect(CLSID_D2D1Composite, &m_composite);

		//산술 복합
		m_deviceContext->CreateEffect(CLSID_D2D1ArithmeticComposite, &m_arithmeticComposite);

		//세피아
		m_deviceContext->CreateEffect(CLSID_D2D1Sepia, &m_sepia);

		//혼합
		m_deviceContext->CreateEffect(CLSID_D2D1Blend, &m_blend);
	}
	else
	{
		MessageBox(m_hWnd, L"DeviceContext Created Failed", NULL, MB_OK);
		return FALSE;
	}

	m_paticleManager = new PaticleMaintain();

	return TRUE;
}

/// <summary>
/// 글자 정렬 지정
/// </summary>
/// <param name="alignment"></param>
void DirectY::SetTextAlignment(TextAlignMent alignment)
{
	switch (alignment)
	{
		case TextAlignMent::LEFT:
			m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

			break;
		case TextAlignMent::RIGHT:
			m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

			break;
		case TextAlignMent::CENTER:
			m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

			break;
		default:
			break;
	}
}

/// <summary>
/// 내부 textformat을 다시 지정한다
/// </summary>
/// <param name="fontsize">폰트 크기 기본 20</param>
/// <param name="msc_fontName">폰트 이름 기본 Verdana</param>
void DirectY::SetTextFormat(const float fontsize /*= 20*/, const WCHAR* msc_fontName /*= L"Verdana"*/)
{
	//textformat 새로 만들기
	m_textFormat->Release();
	m_dWriteFactory->CreateTextFormat(
		msc_fontName,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontsize,
		L"", //locale
		&m_textFormat
	);
}

/// <summary>
/// 이펙트가 적용된 이미지를 비트맵을 바꿔 저장한다
/// </summary>
/// <param name="pImg">적용할 이펙트</param>
/// <param name="sprite">적용할 이미지 및 적용한 이미지를 저장할 곳</param>
/// <returns></returns>
ID2D1Bitmap* DirectY::ImageToBitmap(ID2D1Image* pImg, Graphics::Sprite* const sprite)
{
	ID2D1Image* oldTarget = NULL;
	ID2D1Bitmap1* targetBitmap = NULL;

	UINT32 width = (UINT32)sprite->m_drawSize.right - (UINT32)sprite->m_drawSize.left;
	UINT32 height = (UINT32)sprite->m_drawSize.bottom - (UINT32)sprite->m_drawSize.top;

	// "D2D1_BITMAP_OPTIONS_TARGET" 모드로 적절한 비트맵을 생성.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	HRESULT a = m_deviceContext->CreateBitmap({ width,height }, 0, 0, bitmapProperties, &targetBitmap);

	// 현재 타겟을 저장 해둔다. 그리고 새로 그릴 ID2D1Bitmap 으로 바꿈.
	m_deviceContext->GetTarget(&oldTarget);
	m_deviceContext->SetTarget(targetBitmap);
	// DC 에 Image를 그림.
	m_deviceContext->BeginDraw();
	m_deviceContext->Clear();
	m_deviceContext->DrawImage(pImg);
	m_deviceContext->EndDraw();

	// 비트맵에 해당되는 이미지가 그려졌으므로 해당 비트맵을 리턴한다.
	m_deviceContext->SetTarget(oldTarget);

	oldTarget->Release();

	return targetBitmap;
}

ID2D1Bitmap* DirectY::ImageToBitmap(ID2D1Image* pImg, ID2D1Bitmap* image, UINT32 width, UINT32 height)
{
	ID2D1Image* oldTarget = NULL;
	ID2D1Bitmap1* targetBitmap = NULL;

	// "D2D1_BITMAP_OPTIONS_TARGET" 모드로 적절한 비트맵을 생성.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	HRESULT a = m_deviceContext->CreateBitmap({ width,height }, 0, 0, bitmapProperties, &targetBitmap);

	// 현재 타겟을 저장 해둔다. 그리고 새로 그릴 ID2D1Bitmap 으로 바꿈.
	m_deviceContext->GetTarget(&oldTarget);
	m_deviceContext->SetTarget(targetBitmap);
	// DC 에 Image를 그림.
	m_deviceContext->BeginDraw();
	m_deviceContext->Clear();
	m_deviceContext->DrawImage(pImg);
	m_deviceContext->EndDraw();

	// 비트맵에 해당되는 이미지가 그려졌으므로 해당 비트맵을 리턴한다.
	m_deviceContext->SetTarget(oldTarget);

	oldTarget->Release();

	return targetBitmap;
}

/// <summary>
/// 가득 찬 사각형 그리기
/// </summary>
/// <param name="leftTopX"></param>
/// <param name="leftTopY"></param>
/// <param name="rightBottomX"></param>
/// <param name="rightBottomY"></param>
/// <param name="thickness"></param>
void DirectY::FillRectangle(float leftTopX, float leftTopY, float rightBottomX, float rightBottomY, float thickness)
{
	m_renderTarget->FillRectangle(D2D1::RectF(leftTopX, leftTopY, rightBottomX, rightBottomY), m_tempBrush);
}

