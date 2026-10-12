#include "WinAPIInit.h"
#include "InputManager.h"

WinAPIInit::WinAPIInit()
	:m_hInst(NULL), m_hWnd(NULL)
{

}

WinAPIInit::~WinAPIInit()
{

}

HWND WinAPIInit::Init(HINSTANCE hInstance)
{
	MyRegisterClass(hInstance);
	InitInstance(hInstance);

	return m_hWnd;
}

/// <summary>
/// 
/// </summary>
/// <returns> false: GetMessage WM_QUIT</returns>
MSG WinAPIInit::ProcessMessage()
{
	MSG msg = {};

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
	}

	return msg;
}

void WinAPIInit::Release()
{

}

LRESULT CALLBACK WinAPIInit::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam); // 휠 회전 크기
		//short keyState = GET_KEYSTATE_WPARAM(wParam); // 휠을 돌릴 때의 키 상태 (Ctrl, Shift 등)

		//int x = LOWORD(lParam); // 마우스 X 좌표
		//int y = HIWORD(lParam); // 마우스 Y 좌표

		InputManager::GetInstance().SetWheelPower(zDelta);

		return 0;
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void WinAPIInit::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = hInstance;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"D2DDemoApp";

	RegisterClassEx(&wcex);
}

BOOL WinAPIInit::InitInstance(HINSTANCE hInstance)
{
	m_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	m_hWnd = CreateWindow(
		L"D2DDemoApp",
		L"Direct2D demo application",
		WS_POPUP,
		0,
		0,
		1920,
		1080,
		NULL,
		NULL,
		hInstance,
		nullptr);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	return TRUE;
}
