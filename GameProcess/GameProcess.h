#pragma once

#include <windows.h>

// WinAPI
class WinAPIInit;

// Engine
class DirectY;

// Manager
class GameEngine;

class CollisionManager;
class ID2D1Bitmap;

namespace Graphics { struct Sprite; }

/// <summary>
/// 엔진 초기화, Update 및 Render를 담당한다.
/// </summary>
class GameProcess

{
public:
	GameProcess();
	~GameProcess();

public:
	void Init(HINSTANCE hInstance);
	void Loop();
	void Release();

	void FixedUpdate();
	void Update();
	void LateUpdate();

	void PreRender();
	void Render();
	void PostRender();

	void InitGameScene1();
	void InitGameScene2();
	void InitGameScene3();
	void InitTitleScene();
	void InitOpeningScene();
	void InitStageScene();
	void InitGameOverScene();
	void InitGameClearScene();
	void InitGameCreditScene();
	void InitSecretScene();

private:
	HWND m_hWnd;
	
	WinAPIInit* m_winAPIInit;
	DirectY* m_directY;

	GameEngine* m_gameEngine;
	
	ID2D1Bitmap* mouseCursorImage;
	Graphics::Sprite* mouseCursor;

	ID2D1Bitmap* mouseCursorClickImage;
	Graphics::Sprite* mouseCursorClick;
};

