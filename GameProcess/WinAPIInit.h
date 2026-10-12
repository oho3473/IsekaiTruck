#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <windows.h>

class WinAPIInit
{
public:
	/// ---------- Constructor, Destructor----------
	WinAPIInit();
	~WinAPIInit();

	/// ---------- Event ----------
	HWND Init(HINSTANCE hInstance);
	MSG ProcessMessage();
	void Release();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	/// ---------- Init Window ----------
	void MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance);

private:
	/// ---------- Member ----------
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
};

