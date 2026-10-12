#include "GameProcess.h"

#include "WinAPIInit.h"
#include "DirectY.h"
#include "Sprite.h"

#include "GameEngine.h"

// temp 임시
#include "Scene.h"
#include "Animator.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Button.h"
#include "Scrollbar.h"
#include "AudioSource.h"
#include "Slider.h"

#include "Player.h"
#include "Police.h"
#include "CloudObject.h"

// 타일
#include "TileManager.h"
#include "BuildingManager.h"
#include "TileRenderManager.h"

// 타이머
#include "Timer.h"

// 충돌체
#include "Collider.h"
#include "../PhysicsEngine/IsometricCollider.h"
#include "../PhysicsEngine/VehicleCollider.h"
#include "CollisionManager.h"

//ui
#include "InGameUI.h"
#include "InterviewUI.h"
#include "SoundOptionUI.h"
#include "MainOption.h"
#include "PotraitUI.h"
#include "TargetBar.h"

//NPC
#include "SkinHeadNPC.h"
#include "NpcManager.h"
#include "NPCWaitManager.h"

//Message
#include "MessageManager.h"
#include "DialogManager.h"

// 길찾기
#include "PathFindRenderer.h"

// TitleScene
#include "BackgroundImage.h"

// OpeningScene
#include "Credit.h"

// Stagescene
#include "StageTitle.h"
#include "ClearManager.h"
#include "LockStage.h"

// 디버그
#include "DebugManager.h"

#include <functional>


GameProcess::GameProcess()
	:m_hWnd(NULL), m_winAPIInit(nullptr), m_directY(nullptr),
	m_gameEngine(nullptr)
	, mouseCursorImage(nullptr), mouseCursor(nullptr),
	 mouseCursorClickImage(nullptr), mouseCursorClick(nullptr)
{

}

GameProcess::~GameProcess()
{


}

void GameProcess::Init(HINSTANCE hInstance)
{
	ShowCursor(false);
	m_winAPIInit = new WinAPIInit;
	m_hWnd = m_winAPIInit->Init(hInstance);

	m_directY = new DirectY(m_hWnd);

	m_gameEngine = new GameEngine;
	m_gameEngine->Init(m_hWnd, m_directY);

	m_gameEngine->CreateScene("GameScene1", std::bind(&GameProcess::InitGameScene1, this));
	m_gameEngine->CreateScene("GameScene2", std::bind(&GameProcess::InitGameScene2, this));
	m_gameEngine->CreateScene("GameScene3", std::bind(&GameProcess::InitGameScene3, this));
	m_gameEngine->CreateScene("TitleScene", std::bind(&GameProcess::InitTitleScene, this));
	m_gameEngine->CreateScene("StageScene", std::bind(&GameProcess::InitStageScene, this));
	m_gameEngine->CreateScene("OpeningScene", std::bind(&GameProcess::InitOpeningScene, this));
	m_gameEngine->CreateScene("GameOverScene", std::bind(&GameProcess::InitGameOverScene, this));
	m_gameEngine->CreateScene("GameClearScene", std::bind(&GameProcess::InitGameClearScene, this));
	m_gameEngine->CreateScene("GameCreditScene", std::bind(&GameProcess::InitGameCreditScene, this));
	m_gameEngine->CreateScene("SecretScene", std::bind(&GameProcess::InitSecretScene, this));
	m_gameEngine->LoadScene("TitleScene");

	mouseCursorImage = m_directY->LoadImageFile(L"Resource/mouse2.png");
	mouseCursor = new Graphics::Sprite("mouse", 0, 0, mouseCursorImage, { 0, 0, 88, 88 }, { 0, 0, 88, 88 }, { 0, 0 }, { 0.5f, 0.5f });

	mouseCursorClickImage = m_directY->LoadImageFile(L"Resource/mouse2_tonedown.png");
	mouseCursorClick = new Graphics::Sprite("mouse1_tonedown", 0, 0, mouseCursorClickImage, { 0, 0, 88, 88 }, { 0, 0, 88, 88 }, { 0, 0 }, { 0.5f, 0.5f });

	DebugManager::GetInstance().SetGameEngine(m_gameEngine);
	DebugManager::GetInstance().SetDirectY(m_directY);
}

void GameProcess::Loop()
{
	MSG windowsMessage = {};
	while (true)
	{
		// Update
		windowsMessage = m_winAPIInit->ProcessMessage();

		if (windowsMessage.message == WM_QUIT)
		{
			break;
		}

		FixedUpdate();
		Update();

		PreRender();
		Render();
		PostRender();

		LateUpdate();
	}
}

void GameProcess::Release()
{
	// Engine Release
	delete m_gameEngine;
	delete m_directY;

	delete m_winAPIInit;

	mouseCursorImage->Release();
	delete mouseCursor;
	mouseCursorClickImage->Release();
	delete mouseCursorClick;
}

void GameProcess::FixedUpdate()
{
	m_gameEngine->FixedUpdate();
}

void GameProcess::Update()
{
	if (InputManager::GetInstance().IsGetKeyUp(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if (ClearManager::GetInstance().GetClear())
	{
		m_gameEngine->SetTimeScale(0);
	}

	m_gameEngine->Update();
	DebugManager::GetInstance().Update(m_gameEngine->GetDeltaTime());
}

void GameProcess::LateUpdate()
{
	m_gameEngine->LateUpdate();
}

void GameProcess::PreRender()
{
	m_gameEngine->PreRender();
}

void GameProcess::Render()
{
	m_directY->BeginDraw();

	// 화면 초기화
	m_directY->SetTransform({ 1,0,0,1,0,0 });
	m_directY->CleanWindow();

	m_gameEngine->Render(m_directY);

	// 화면 초기화
	m_directY->SetTransform({ 1,0,0,1,0,0 });

	if (InputManager::GetInstance().IsGetKey(VK_LBUTTON))
	{
		mouseCursorClick->m_drawPos = { InputManager::GetInstance().GetMousePosition().x - 29, InputManager::GetInstance().GetMousePosition().y - 15 };
		m_directY->DrawBitmap(mouseCursorClick);
	}
	else
	{
		mouseCursor->m_drawPos = { InputManager::GetInstance().GetMousePosition().x - 29, InputManager::GetInstance().GetMousePosition().y - 15 };
		m_directY->DrawBitmap(mouseCursor);
	}
	//m_directY->CleanWindow();

	if (DebugManager::GetInstance().GetIsDebuging())
	{
		m_directY->SetBrushColor(255, 255, 255, 255);
		m_directY->DrawText(50, 20, 20, TextAlignMent::LEFT, L"verdana", "FPS: %d", m_gameEngine->GetFPS());
		m_directY->DrawText(50, 40, 20, TextAlignMent::LEFT, L"verdana", "DT: %f", m_gameEngine->GetDeltaTime());
		m_directY->DrawText(50, 60, 20, TextAlignMent::LEFT, L"verdana", "MouseX: %d", (int)InputManager::GetInstance().GetMousePosition().x);
		m_directY->DrawText(200, 60, 20, TextAlignMent::LEFT, L"verdana", "MouseY: %d", (int)InputManager::GetInstance().GetMousePosition().y);
	}
}

void GameProcess::PostRender()
{
	m_gameEngine->PostRender(m_directY);
	m_directY->EndDraw();
}

void GameProcess::InitGameScene1()
{
	Scene* thisScene = m_gameEngine->FindScene("GameScene1");
	m_gameEngine->LoadResource("GameScene", m_directY);

	m_gameEngine->SetTimeScale(0);

	/// 테스트용 bgm재생	
	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("bgm_ingame_01_loop"),1.f,PlayType::BGM);

#pragma region DialogManager
 	GameObject* dialogManagerObject = m_gameEngine->CreateObject("DialogManager", thisScene);
	Transform* dialogManagerTransform = new Transform();
	dialogManagerObject->SetElement(dialogManagerTransform, ElementType::Transform);
	dialogManagerTransform->SetUseCamera(false);
	dialogManagerTransform->SetLayer(10000);

	Graphics::Sprite* dialogImage = m_directY->ReadSpriteTxtFile("dialog", m_gameEngine->GetImage("dialog"));
	dialogImage->m_drawPos = { 37, 775 };

	Graphics::Sprite* dialogGodImage = m_directY->ReadSpriteTxtFile("dialogGod", m_gameEngine->GetImage("dialogGod"));
	dialogGodImage->m_drawPos = { 30, 760 };

	DialogManager* dialogManager = new DialogManager(0, 0, 1396, 1080, 1000, dialogImage, dialogGodImage);
	dialogManagerObject->SetElement(dialogManager, ElementType::Script);
	dialogManager->ReadDialogCSV("Resource/Dialog2");
	dialogManager->SetTextOffset({ 130, 860 });

	Graphics::Sprite* goddessImageSprite = m_directY->ReadSpriteTxtFile("goddessStanding", m_gameEngine->GetImage("goddess"));
	Graphics::Sprite* goddessDarkImageSprite = m_directY->ReadSpriteTxtFile("goddessStanding", m_gameEngine->GetImage("goddess_Dark"));
	Graphics::Sprite* playerImageSprite = m_directY->ReadSpriteTxtFile("playerStanding", m_gameEngine->GetImage("Player"));
	Graphics::Sprite* playerDarkImageSprite = m_directY->ReadSpriteTxtFile("playerStanding", m_gameEngine->GetImage("Player_Dark"));

	dialogManager->AddSprite("goddess", goddessImageSprite);
	dialogManager->AddSprite("goddess_Dark", goddessDarkImageSprite);
	dialogManager->AddSprite("Player", playerImageSprite);
	dialogManager->AddSprite("Player_Dark", playerDarkImageSprite);

	AudioSource* dialogManagerAudioSource = new AudioSource(m_gameEngine);
	dialogManagerObject->SetElement(dialogManagerAudioSource, ElementType::AudioSource);
	dialogManager->SetAudioSource(dialogManagerAudioSource);
	dialogManagerAudioSource->AddSound("sfx_dialog_1");

#pragma endregion DialogManager

#pragma region UI

#pragma region MainUI
	/// MainUIWindow
	GameObject* UIObject = m_gameEngine->CreateObject("UI", thisScene);
	Transform* uiTransform = new Transform();
	UIObject->SetElement(uiTransform, ElementType::Transform);
	uiTransform->SetLocalPosition({ 1398,40 });
	uiTransform->SetLayer(300);

	InGameUI* mainUI = new InGameUI();
	UIObject->SetElement(mainUI, ElementType::Script);
	Graphics::Sprite* UIImage = m_directY->ReadSpriteTxtFile("ingame_mainsolid", m_gameEngine->GetImage("ingame_mainsolid"));
	mainUI->m_sprite = UIImage;
#pragma endregion MainUI

#pragma region HeroUI
	///면접자 초상화UI
	GameObject* heroPotraitObject = m_gameEngine->CreateObject("heroPotrait", thisScene);
	Transform* heroPotraitTransform = new Transform();
	heroPotraitObject->SetElement(heroPotraitTransform, ElementType::Transform);
	heroPotraitTransform->SetParent(uiTransform);
	heroPotraitTransform->Translate({ 25,446 });
	heroPotraitTransform->SetLayer(312);

	InterviewUI* heroPotraitUI = new InterviewUI();
	heroPotraitObject->SetElement(heroPotraitUI, ElementType::Script);
	Graphics::Motion* interviewImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("heroPotrait", m_gameEngine->GetImage("heroPotrait"), interviewImage);
	heroPotraitUI->SetImage(interviewImage);

	///면접자 얼굴 이미지
	GameObject* heroFaceObject = m_gameEngine->CreateObject("heroPotrait", thisScene);
	Transform* heroFaceTransform = new Transform();
	heroFaceObject->SetElement(heroFaceTransform, ElementType::Transform);
	heroFaceTransform->SetParent(uiTransform);
	heroFaceTransform->Translate({ 35,446 });
	heroFaceTransform->SetLayer(313);

	PotraitUI* heroFaceUI = new PotraitUI();
	heroFaceObject->SetElement(heroFaceUI, ElementType::Script);
	Graphics::Motion* potraitImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("potraitGroup", m_gameEngine->GetImage("potraitGroup"),potraitImage);
	heroFaceUI->SetPotraitGroup(potraitImage);

	///면접자 초상화 프레임
	GameObject* heroFrameObject = m_gameEngine->CreateObject("heroframe", thisScene);
	Transform* heroFrameTransform = new Transform();
	heroFrameObject->SetElement(heroFrameTransform, ElementType::Transform);
	heroFrameTransform->SetParent(uiTransform);
	heroFrameTransform->SetLocalPosition({ 25,446 });
	heroFrameTransform->SetLayer(314);

	InGameUI* heroFrameUI = new InGameUI();
	heroFrameObject->SetElement(heroFrameUI, ElementType::Script);
	Graphics::Sprite* heroFrameImage = m_directY->ReadSpriteTxtFile("heroframe", m_gameEngine->GetImage("heroframe"));
	heroFrameUI->m_sprite = heroFrameImage;
#pragma endregion HeroUI

#pragma region GoddessUI
	///여신 초상화
	GameObject* goddessPotraitObject = m_gameEngine->CreateObject("goddessPotrait", thisScene);
	Transform* goddessPotraitTransform = new Transform();
	goddessPotraitObject->SetElement(goddessPotraitTransform, ElementType::Transform);
	goddessPotraitTransform->SetParent(uiTransform);
	goddessPotraitTransform->Translate({ 305,446 });
	goddessPotraitTransform->SetLayer(312);

	InterviewUI* goddesspotraitUI = new InterviewUI();
	goddessPotraitObject->SetElement(goddesspotraitUI, ElementType::Script);
	Graphics::Motion* goddessImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("goddessPotrait", m_gameEngine->GetImage("goddessPotrait"), goddessImage);
	goddesspotraitUI->SetImage(goddessImage);

	///여신 얼굴 이미지
	GameObject* goddessFaceObject = m_gameEngine->CreateObject("goddessPotrait", thisScene);
	Transform* goddessFaceTransform = new Transform();
	goddessFaceObject->SetElement(goddessFaceTransform, ElementType::Transform);
	goddessFaceTransform->SetParent(uiTransform);
	goddessFaceTransform->Translate({ 315,453 });
	goddessFaceTransform->SetLayer(313);

	InterviewUI* goddessFaceUI = new InterviewUI();
	goddessFaceObject->SetElement(goddessFaceUI, ElementType::Script);
	Graphics::Motion* goddessFaceImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("portrait", m_gameEngine->GetImage("portrait_godness"), goddessFaceImage);
	goddessFaceUI->SetImage(goddessFaceImage);

	///여신 초상화 프레임
	GameObject* goddessBarFrameObject = m_gameEngine->CreateObject("goddessbarframe", thisScene);
	Transform* goddessBarFrameTransform = new Transform();
	goddessBarFrameObject->SetElement(goddessBarFrameTransform, ElementType::Transform);
	goddessBarFrameTransform->SetParent(uiTransform);
	goddessBarFrameTransform->Translate({ 305,446 });
	goddessBarFrameTransform->SetLayer(314);

	InterviewUI* goddessBarFrameUI = new InterviewUI();
	goddessBarFrameObject->SetElement(goddessBarFrameUI, ElementType::Script);
	Graphics::Motion* goddessBarFrameImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("goddessbarframe", m_gameEngine->GetImage("goddessbarframe"), goddessBarFrameImage);
	goddessBarFrameUI->SetImage(goddessBarFrameImage);
#pragma endregion GoddessUI

#pragma region TimerUI
	///Timer
	GameObject* TimerObject = m_gameEngine->CreateObject("Timer", thisScene);
	Transform* TimerTransform = new Transform();
	TimerObject->SetElement(TimerTransform, ElementType::Transform);
	// 부모트랜스폼 설정
	// 트랜스폼 레이어 설정(일단 보일 수 있도록 높은 숫자 넣음)
	TimerTransform->SetLayer(314);
	Timer* timer = new Timer(m_gameEngine, m_directY);
	TimerObject->SetElement(timer, ElementType::Script);

	// 오디오 소스를 추가함
	AudioSource* timerAudioSource = new AudioSource(m_gameEngine);
	TimerObject->SetElement(timerAudioSource, ElementType::AudioSource);
	timer->SetAudioSource(timerAudioSource);
	timerAudioSource->AddSound("bgm_beep_slow");
	timerAudioSource->AddSound("sfx_timer_warning");

	///타이머 장식
	GameObject* secondBaseObject = m_gameEngine->CreateObject("secondHandBase", thisScene);
	Transform* secondBaseTransform = new Transform();
	secondBaseTransform->SetParent(uiTransform);
	secondBaseTransform->SetLocalPosition({ 205 , 622 });
	secondBaseObject->SetElement(secondBaseTransform, ElementType::Transform);
	secondBaseTransform->SetLayer(315);

	InGameUI* secondBaseUI = new InGameUI();
	secondBaseObject->SetElement(secondBaseUI, ElementType::Script);
	Graphics::Sprite* secondBaseImage = m_directY->ReadSpriteTxtFile("secondhand_base", m_gameEngine->GetImage("secondhand_base"));
	secondBaseUI->m_sprite = secondBaseImage;

	///타이머 프레임
	GameObject* clockbaseObject = m_gameEngine->CreateObject("clockbase", thisScene);
	Transform* clockbaseTransform = new Transform();
	clockbaseTransform->SetParent(uiTransform);
	clockbaseTransform->SetLocalPosition({ 120 , 652 });
	clockbaseObject->SetElement(clockbaseTransform, ElementType::Transform);
	clockbaseTransform->SetLayer(314);

	InGameUI* clockbaseUI = new InGameUI();
	clockbaseObject->SetElement(clockbaseUI, ElementType::Script);
	Graphics::Sprite* clockbaseImage = m_directY->ReadSpriteTxtFile("clockbase", m_gameEngine->GetImage("clockbase"));
	clockbaseUI->m_sprite = clockbaseImage;
	dialogManager->SetStartGame([=]() 
		{
			m_gameEngine->SetTimeScale(1);
			timer->StartTimer();

		});

#pragma endregion TimerUI

#pragma region MessageUI
	/// 메세지 창 Scrollbar
	GameObject* dialogueObject = m_gameEngine->CreateObject("Scrollbar", thisScene);
	Transform* dialogueTrasform = new Transform();
	dialogueObject->SetElement(dialogueTrasform, ElementType::Transform);
	dialogueTrasform->SetParent(uiTransform);
	dialogueTrasform->Translate({ 25, 35 });
	dialogueTrasform->SetLayer(310);

	Graphics::Sprite* bar = m_directY->ReadSpriteTxtFile("bar", m_gameEngine->GetImage("bar"));
	Graphics::Sprite* background = m_directY->ReadSpriteTxtFile("message", m_gameEngine->GetImage("message"));
	Graphics::Motion* herochatbubble = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("herochatwindow", m_gameEngine->GetImage("chatwindow"), herochatbubble);

	Graphics::Motion* goddesschatbubble = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("herochatwindow", m_gameEngine->GetImage("chatwindow_goddess"), goddesschatbubble);

	Scrollbar* scrollbar = new Scrollbar(35, 50, 422 - 35, 380 - 50, background, bar, herochatbubble,goddesschatbubble);
	dialogueObject->SetElement(scrollbar, ElementType::Scrollbar);
	scrollbar->SetHeroChatWindow(herochatbubble);
	scrollbar->SetGoddessChatWindow(goddesschatbubble);

	/*///메세지 프레임
	GameObject* messageFrameObject = m_gameEngine->CreateObject("heroframe", thisScene);
	Transform* messageFrameTransform = new Transform();
	messageFrameObject->SetElement(messageFrameTransform, ElementType::Transform);
	messageFrameTransform->SetParent(uiTransform);
	messageFrameTransform->SetLocalPosition({ 25,35 });
	messageFrameTransform->SetLayer(312);

	InGameUI* messageFrameUI = new InGameUI();
	messageFrameObject->SetElement(messageFrameUI, ElementType::Script);
	Graphics::Sprite* messageFrameImage = m_directY->ReadSpriteTxtFile("messageframe", m_gameEngine->GetImage("messageframe"));
	messageFrameUI->m_sprite = messageFrameImage;*/
#pragma endregion MessageUI

#pragma region InterviewRoomUI
	///면접장
	GameObject* InterviewRoomObject = m_gameEngine->CreateObject("interviewPlace", thisScene);
	Transform* InterviewRoomTransform = new Transform();
	InterviewRoomTransform->SetParent(uiTransform);
	InterviewRoomTransform->SetLocalPosition({ 36, 672 });
	InterviewRoomObject->SetElement(InterviewRoomTransform, ElementType::Transform);
	InterviewRoomTransform->SetLayer(310);

	InterviewUI* InterviewRoomUI = new InterviewUI();
	InterviewRoomObject->SetElement(InterviewRoomUI, ElementType::Script);
	Graphics::Motion* interviewPlaceImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_bar", m_gameEngine->GetImage("interview_screen_bar"), interviewPlaceImage);
	InterviewRoomUI->SetImage( interviewPlaceImage);

	/// 면접장 배경
	GameObject* InterviewRoomBGObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewRoomBGTransform = new Transform();
	InterviewRoomBGTransform->SetParent(uiTransform);
	InterviewRoomBGTransform->SetLocalPosition({ 35, 673 });
	InterviewRoomBGObject->SetElement(InterviewRoomBGTransform, ElementType::Transform);
	InterviewRoomBGTransform->SetLayer(311);

	InterviewUI* InterviewRoomBGUI = new InterviewUI();
	InterviewRoomBGObject->SetElement(InterviewRoomBGUI, ElementType::Script);
	Graphics::Motion* interviewRoomImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("InterviewRoomBackground", m_gameEngine->GetImage("InterviewRoomBackground"), interviewRoomImage);
	InterviewRoomBGUI->SetImage( interviewRoomImage);

	///면접장 빛
	GameObject* InterviewLightObject = m_gameEngine->CreateObject("interviewLight", thisScene);
	Transform* InterviewLightTransform = new Transform();
	InterviewLightTransform->SetParent(uiTransform);
	InterviewLightTransform->SetLocalPosition({ 35, 673 });
	InterviewLightObject->SetElement(InterviewLightTransform, ElementType::Transform);
	InterviewLightTransform->SetLayer(312);

	InterviewUI* InterviewLightUI = new InterviewUI();
	InterviewLightObject->SetElement(InterviewLightUI, ElementType::Script);
	Graphics::Motion* interviewLightImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interviewLight", m_gameEngine->GetImage("interviewLight"), interviewLightImage);
	InterviewLightUI->SetImage( interviewLightImage);

	/// 면접장 프레임
	GameObject* InterviewFrameObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewFrameTransform = new Transform();
	InterviewFrameTransform->SetParent(uiTransform);
	InterviewFrameTransform->SetLocalPosition({ 25, 535 });
	InterviewFrameObject->SetElement(InterviewFrameTransform, ElementType::Transform);
	InterviewFrameTransform->SetLayer(312);

	InterviewUI* InterviewFrameUI = new InterviewUI();
	InterviewFrameObject->SetElement(InterviewFrameUI, ElementType::Script);
	Graphics::Motion* interviewFrameImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_frame", m_gameEngine->GetImage("interview_screen_frame"), interviewFrameImage);
	InterviewFrameUI->SetImage( interviewFrameImage);

	///면접장 데코
	GameObject* InterviewDecoObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewDecoTransform = new Transform();
	InterviewDecoTransform->SetParent(uiTransform);
	InterviewDecoTransform->SetLocalPosition({ 20, 652 });
	InterviewDecoObject->SetElement(InterviewDecoTransform, ElementType::Transform);
	InterviewDecoTransform->SetLayer(313);

	InterviewUI* InterviewDecoUI = new InterviewUI();
	InterviewDecoObject->SetElement(InterviewDecoUI, ElementType::Script);
	Graphics::Motion* interviewDecoImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_frame_deco", m_gameEngine->GetImage("interview_screen_frame_deco"), interviewDecoImage);
	InterviewDecoUI->SetImage( interviewDecoImage);
#pragma endregion InterviewRoomUI

#pragma  region MainOptionUI

	GameObject* MainOptionUIObject = m_gameEngine->CreateObject("MainOptionUI", thisScene);
	Transform* MainOptionUITransform = new Transform;
	MainOptionUIObject->SetElement(MainOptionUITransform, ElementType::Transform);
	MainOptionUITransform->SetLayer(500);
	MainOptionUITransform->SetLocalPosition({ 960 - 315,540 - 215 });
	MainOptionUIObject->SetActive(false);

	MainOption* mainOptionUI = new MainOption();
	MainOptionUIObject->SetElement(mainOptionUI, ElementType::Script);
	Graphics::Sprite* mainOptionImage = m_directY->ReadSpriteTxtFile("menubar_all", m_gameEngine->GetImage("menubar_all"));
	mainOptionUI->SetImage(mainOptionImage);

	/// 타이틀로 가기 옵션 버튼
	GameObject* mainButtonObject = m_gameEngine->CreateObject("MainTitleButton", thisScene);
	mainButtonObject->SetActive(false);

	Transform* mainButtonTransform = new Transform();
	mainButtonObject->SetElement(mainButtonTransform, ElementType::Transform);
	mainButtonTransform->SetParent(MainOptionUITransform);
	mainButtonTransform->SetLocalPosition({ 185, 125 });

	Vector2 mainButtonWorldPosition = mainButtonTransform->GetWorldPosition();
	Button* mainButton = new Button(0 + (int)mainButtonWorldPosition.x, 0 + (int)mainButtonWorldPosition.y, 266 + (int)mainButtonWorldPosition.x, 71 + (int)mainButtonWorldPosition.y);
	mainButtonObject->SetElement(mainButton, ElementType::Script);
	mainButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("TitleScene");
			}
		});
	
	// 메인버튼 오디오소스
	AudioSource* mainButtonAudioSource = new AudioSource(m_gameEngine);
	mainButtonObject->SetElement(mainButtonAudioSource, ElementType::AudioSource);
	mainButton->SetAudioSource(mainButtonAudioSource);
	mainButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	/// 다시하기 옵션 버튼
	GameObject* replayButtonObject = m_gameEngine->CreateObject("ReplayButton", thisScene);
	replayButtonObject->SetActive(false);

	Transform* replayButtonTransform = new Transform();
	replayButtonObject->SetElement(replayButtonTransform, ElementType::Transform);
	replayButtonTransform->SetParent(MainOptionUITransform);
	replayButtonTransform->SetLocalPosition({ 185, 215 });

	Vector2 replaybuttonWorldPosition = replayButtonTransform->GetWorldPosition();
	Button* replayButton = new Button(0 + (int)replaybuttonWorldPosition.x, 0 + (int)replaybuttonWorldPosition.y, 266 + (int)replaybuttonWorldPosition.x, 71 + (int)replaybuttonWorldPosition.y);
	replayButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("GameScene1");
			}
		});
	replayButtonObject->SetElement(replayButton, ElementType::Script);

	// 다시하기 버튼 오디오소스
	AudioSource* replayButtonAudioSource = new AudioSource(m_gameEngine);
	replayButtonObject->SetElement(replayButtonAudioSource, ElementType::AudioSource);
	replayButton->SetAudioSource(replayButtonAudioSource);
	replayButtonAudioSource->AddSoundVector("sfx_OnButton");
	replayButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	///옵션 닫기
	GameObject* exitButtonObject = m_gameEngine->CreateObject("exitbutton", thisScene);

	Transform* exitbuttonTransform = new Transform();
	exitButtonObject->SetElement(exitbuttonTransform, ElementType::Transform);
	exitbuttonTransform->SetLayer(501);
	exitButtonObject->SetActive(false);

	Button* exitButton = new Button(1240, 325, 1276, 360);
	Graphics::Sprite* exiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	exitButton->m_sprite = exiteButtonImage;
	exitButtonObject->SetElement(exitButton, ElementType::Button);
	
	AudioSource* exitButtonAudioSource = new AudioSource(m_gameEngine);
	exitButtonObject->SetElement(exitButtonAudioSource, ElementType::AudioSource);
	exitButton->SetAudioSource(exitButtonAudioSource);
	exitButtonAudioSource->AddSoundVector("sfx_OnButton");
	exitButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	/// 우상단 옵션 버튼
	GameObject* MainButtonObject = m_gameEngine->CreateObject("mainbutton", thisScene);
	Transform* MainButtonTransform = new Transform();
	MainButtonObject->SetElement(MainButtonTransform, ElementType::Transform);
	MainButtonTransform->SetLayer(500);

	Button* MainOptionButton = new Button(1865, 15, 1905, 55);
	Graphics::Sprite* MainButtonImage = m_directY->ReadSpriteTxtFile("button", m_gameEngine->GetImage("menubutton"));
	MainOptionButton->m_sprite = MainButtonImage;
	MainButtonObject->SetElement(MainOptionButton, ElementType::Button);

	AudioSource* mainOptionButtonAudioSource = new AudioSource(m_gameEngine);
	MainButtonObject->SetElement(mainOptionButtonAudioSource, ElementType::AudioSource);
	MainOptionButton->SetAudioSource(mainOptionButtonAudioSource);
	mainOptionButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainOptionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion MainOptionUI	

#pragma region SoundOptionUI
	/// 옵션UI생성
	/// OptionUIObject 생성
	GameObject* SoundOptionUIObject = m_gameEngine->CreateObject("optionUI", thisScene);
	Transform* soundOptionUITransform = new Transform;
	SoundOptionUIObject->SetElement(soundOptionUITransform, ElementType::Transform);
	soundOptionUITransform->SetLayer(510);
	soundOptionUITransform->SetLocalPosition({ (1920 / 2) - 315.5f ,(1080 / 2) - 215.5f });

	// 볼륨슬라이더 오브젝트 생성
	GameObject* MasterVolumeSliderObject = m_gameEngine->CreateObject("MsterVolumeSlider", thisScene);
	GameObject* BGMVolumeSliderObject = m_gameEngine->CreateObject("BGMVolumeSlider", thisScene);
	GameObject* SFXVolumeSliderObject = m_gameEngine->CreateObject("SFXVolumeSlider", thisScene);

	// 슬라이더 트랜스폼
	Transform* masterVolumeSliderTransform = new Transform;
	Transform* BGMVolumeSliderTransform = new Transform;
	Transform* SFXVolumeSliderTransform = new Transform;

	MasterVolumeSliderObject->SetElement(masterVolumeSliderTransform, ElementType::Transform);
	BGMVolumeSliderObject->SetElement(BGMVolumeSliderTransform, ElementType::Transform);
	SFXVolumeSliderObject->SetElement(SFXVolumeSliderTransform, ElementType::Transform);

	masterVolumeSliderTransform->SetParent(soundOptionUITransform);
	BGMVolumeSliderTransform->SetParent(soundOptionUITransform);
	SFXVolumeSliderTransform->SetParent(soundOptionUITransform);

	masterVolumeSliderTransform->SetLayer(511);
	BGMVolumeSliderTransform->SetLayer(511);
	SFXVolumeSliderTransform->SetLayer(511);

	// 위치를 미리 찍는다.
	masterVolumeSliderTransform->SetLocalPosition({ 160, 100 });
	BGMVolumeSliderTransform->SetLocalPosition({ 160, 200 });
	SFXVolumeSliderTransform->SetLocalPosition({ 160, 300 });

	// 볼륨슬라이더 리소스 생성
	// (슬라이더 리소스들의 해제가 이루어지지 않고 있음)
	// ui
	Graphics::Sprite* soundOptionUIBackground = m_directY->ReadSpriteTxtFile("sounduibar", m_gameEngine->GetImage("sounduibar"));
	// slider
	Graphics::Sprite* sliderBackground_Master = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_BGM = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_SFX = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderFillArea_Master = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_Master = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_BGM = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_BGM = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_SFX = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_SFX = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));

	Graphics::Sprite* BGImage = m_directY->ReadSpriteTxtFile("bgsound", m_gameEngine->GetImage("bgsound")); //배경음 글자 이미지
	Graphics::Sprite* EffectImage = m_directY->ReadSpriteTxtFile("effectsound", m_gameEngine->GetImage("effectsound")); //효과음 글자 이미지
	Graphics::Sprite* MasterImage = m_directY->ReadSpriteTxtFile("mastersound", m_gameEngine->GetImage("mastersound")); //마스터 글자 이미지

	// 슬라이더 AA
	Slider* masterVolumeSlider = new Slider((int)sliderBackground_Master->m_drawSize.left, (int)sliderBackground_Master->m_drawSize.right,
		(int)sliderBackground_Master->m_drawSize.top, (int)sliderBackground_Master->m_drawSize.bottom,
		(int)(sliderBackground_Master->m_drawSize.left + sliderBackground_Master->m_drawSize.right) / 2,
		(int)(sliderBackground_Master->m_drawSize.top, sliderBackground_Master->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(MASTER));
	Slider* SFXVolumeSlider = new Slider((int)sliderBackground_SFX->m_drawSize.left, (int)sliderBackground_SFX->m_drawSize.right,
		(int)sliderBackground_SFX->m_drawSize.top, (int)sliderBackground_SFX->m_drawSize.bottom,
		(int)(sliderBackground_SFX->m_drawSize.left + sliderBackground_SFX->m_drawSize.right) / 2,
		(int)(sliderBackground_SFX->m_drawSize.top, sliderBackground_SFX->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(SFX));
	Slider* BGMVolumeSlider = new Slider((int)sliderBackground_BGM->m_drawSize.left, (int)sliderBackground_BGM->m_drawSize.right,
		(int)sliderBackground_BGM->m_drawSize.top, (int)sliderBackground_BGM->m_drawSize.bottom,
		(int)(sliderBackground_BGM->m_drawSize.left + sliderBackground_BGM->m_drawSize.right) / 2,
		(int)(sliderBackground_BGM->m_drawSize.top, sliderBackground_BGM->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(BGM));

	// 슬라이더 이미지 저장
	masterVolumeSlider->SetBackground(sliderBackground_Master);
	masterVolumeSlider->SetFillArea(sliderFillArea_Master);
	masterVolumeSlider->SetHandle(sliderBar_Master);

	BGMVolumeSlider->SetBackground(sliderBackground_BGM);
	BGMVolumeSlider->SetFillArea(sliderFillArea_BGM);
	BGMVolumeSlider->SetHandle(sliderBar_BGM);

	SFXVolumeSlider->SetBackground(sliderBackground_SFX);
	SFXVolumeSlider->SetFillArea(sliderFillArea_SFX);
	SFXVolumeSlider->SetHandle(sliderBar_SFX);

	MasterVolumeSliderObject->SetElement(masterVolumeSlider, ElementType::Slider);
	BGMVolumeSliderObject->SetElement(BGMVolumeSlider, ElementType::Slider);
	SFXVolumeSliderObject->SetElement(SFXVolumeSlider, ElementType::Slider);

	// 슬라이더 오브젝트만 가져서 수치 반영만 한다.
	SoundOptionUI* soundOptionUI = new SoundOptionUI(masterVolumeSlider, BGMVolumeSlider, SFXVolumeSlider);
	SoundOptionUIObject->SetElement(soundOptionUI, ElementType::Script);
	soundOptionUI->SetUISprite(soundOptionUIBackground);
	soundOptionUI->SetMasterTiltle(MasterImage);
	soundOptionUI->SetBgmTiltle(BGImage);
	soundOptionUI->SetSfxTiltle(EffectImage);

	soundOptionUI->Init();

	/// 사운드 옵션 끄기 버튼
	GameObject* soundExitButtonObject = m_gameEngine->CreateObject("soundExitbutton", thisScene);
	Transform* soundExitbuttonTransform = new Transform();
	soundExitButtonObject->SetElement(soundExitbuttonTransform, ElementType::Transform);
	soundExitbuttonTransform->SetLocalPosition({ 597, 0 });
	soundExitbuttonTransform->SetLayer(511);
	soundExitButtonObject->SetActive(false);
	soundExitButtonObject->SetIsDraw(false);

	Button* soundExitButton = new Button(1240, 325, 1275, 360);
	Graphics::Sprite* soundExiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	soundExitButton->m_sprite = soundExiteButtonImage;
	soundExitButton->SetOnClick([=]()
		{
			if (SoundOptionUIObject->GetActive())
			{
				MainOptionUIObject->SetActive(true);

				soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
				exitButtonObject->SetActive(!exitButtonObject->GetActive());
				SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetActive());
				MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
				SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
				BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
			}
		});
	soundExitButtonObject->SetElement(soundExitButton, ElementType::Button);

	AudioSource* soundExitButtonAudiosource = new AudioSource(m_gameEngine);
	soundExitButtonObject->SetElement(soundExitButtonAudiosource, ElementType::AudioSource);
	soundExitButton->SetAudioSource(soundExitButtonAudiosource);
	soundExitButtonAudiosource->AddSoundVector("sfx_OnButton");
	soundExitButtonAudiosource->AddSoundVector("sfx_StageSelect_click");

	/// 음량 옵션 버튼
	GameObject* ButtonObject = m_gameEngine->CreateObject("soundbutton", thisScene);
	ButtonObject->SetActive(false);

	Transform* buttonTransform = new Transform();
	ButtonObject->SetElement(buttonTransform, ElementType::Transform);
	buttonTransform->SetParent(MainOptionUITransform);
	buttonTransform->SetLocalPosition({ 185, 310 });
	buttonTransform->SetLayer(501);

	Button* soundOptionButton = new Button(830, 635, 1095, 705);
	//Graphics::Sprite* buttonImage = m_directY->ReadSpriteTxtFile("soundbutton", m_gameEngine->GetImage("soundbutton"));
	//soundOptionButton->m_sprite = buttonImage;
	soundOptionButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				MainOptionUIObject->SetActive(false);

				exitButtonObject->SetActive(!exitButtonObject->GetActive());
				soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
				SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetIsDraw());
				MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
				SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
				BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
			}
		});
	ButtonObject->SetElement(soundOptionButton, ElementType::Button);

	AudioSource* soundOptionButtonAudioSource = new AudioSource(m_gameEngine);
	ButtonObject->SetElement(soundOptionButtonAudioSource, ElementType::AudioSource);
	soundOptionButton->SetAudioSource(soundOptionButtonAudioSource);
	soundOptionButtonAudioSource->AddSoundVector("sfx_OnButton");
	soundOptionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion SoundOptionUI

#pragma region OnClick
	MainOptionButton->SetOnClick([=]()
		{
			MainOptionUIObject->SetActive(true);
			exitButtonObject->SetActive(true);

			mainButtonObject->SetActive(true);
			replayButtonObject->SetActive(true);
			exitButtonObject->SetActive(true);
			ButtonObject->SetActive(true);
		});
	exitButton->SetOnClick([=]()
		{
			MainOptionUIObject->SetActive(false);
			exitButtonObject->SetActive(false);

			mainButtonObject->SetActive(false);
			replayButtonObject->SetActive(false);
			exitButtonObject->SetActive(false);
			ButtonObject->SetActive(false);
		});
#pragma endregion OnClick

#pragma endregion UI

#pragma region Player
	GameObject* PlayerObject = m_gameEngine->CreateObject("Player", thisScene);

	Transform* playerTransform = new Transform();
	PlayerObject->SetElement(playerTransform, ElementType::Transform);
	playerTransform->SetUseCamera(true);
	playerTransform->SetLayer(5);
	playerTransform->SetLocalPosition({ 202.f, 1300.f });

	Player* player = new Player();
	PlayerObject->SetElement(player, ElementType::Script);
	player->SetMyTransform(playerTransform);

	Graphics::Motion* truckMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("isekaiTruck", m_gameEngine->GetImage("Truck"), truckMotion);
	player->SetMotion(truckMotion);

	player->SetMyPaticle(m_directY->ReadSpriteTxtFile("starB", m_gameEngine->GetImage("starB")));
	player->SetMyPaticle2(m_directY->ReadSpriteTxtFile("starM", m_gameEngine->GetImage("starM")));

	for (int i = 0; i < 32; i++)
	{
		truckMotion->GetSprite(i)->m_drawPos.x -= truckMotion->GetSprite(i)->m_pivot.x;
		truckMotion->GetSprite(i)->m_drawPos.y -= truckMotion->GetSprite(i)->m_pivot.y;
	}

	Collider* playerCollider = m_gameEngine->GetCollisionManager()->CreateVehicleCollider(0.f, 0.f, 100, 200.f);
	m_gameEngine->GetCollisionManager()->SetPlayerCollider(playerCollider);
	m_gameEngine->GetCollisionManager()->SetPlayerObject(PlayerObject);
	PlayerObject->SetElement(playerCollider, ElementType::Collider);

	AudioSource* playerAudioScource = new AudioSource(m_gameEngine);
	PlayerObject->SetElement(playerAudioScource, ElementType::AudioSource);
	player->SetAudioSource(playerAudioScource);
	playerAudioScource->AddSound("sfx_hit_car_1");
	playerAudioScource->AddSound("sfx_hit_car_1");
	playerAudioScource->AddSound("sfx_hit_npc_1");
	playerAudioScource->AddSound("sfx_hit_npc_1");
	playerAudioScource->AddSound("sfx_hit_wall_1");
	playerAudioScource->AddSound("sfx_truck_start");
	playerAudioScource->AddSound("sfx_engineSoundMId_loop");
	playerAudioScource->AddSound("sfx_engineSoundBase_loop");
	playerAudioScource->AddSound("sfx_trucBack_loop");

#pragma endregion Player

#pragma region GroundRenderManager
	GameObject* GroundRenderManagerObject = m_gameEngine->CreateObject("GroundRenderManager", thisScene);
	Transform* GroundRenderManagerTransform = new Transform();
	GroundRenderManagerObject->SetElement(GroundRenderManagerTransform, ElementType::Transform);
	GroundRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	GroundRenderManagerTransform->SetLayer(0);
	TileRenderManager* groundRenderManager = new TileRenderManager(m_gameEngine,thisScene, m_directY, TileRenderState::Ground);
	GroundRenderManagerObject->SetElement(groundRenderManager, ElementType::Script);
	groundRenderManager->Init();
	groundRenderManager->ReadSource("Resource/Text/Map/Stage1_tile.txt");

#pragma endregion GroundRenderManager
#pragma region ZoneRenderManager
	GameObject* ZoneRenderManagerObject = m_gameEngine->CreateObject("ZoneRenderManager", thisScene);
	Transform* ZoneRenderManagerTransform = new Transform();
	ZoneRenderManagerObject->SetElement(ZoneRenderManagerTransform, ElementType::Transform);
	ZoneRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	ZoneRenderManagerTransform->SetLayer(0);
	TileRenderManager* ZoneRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY, TileRenderState::Ground);
	ZoneRenderManagerObject->SetElement(ZoneRenderManager, ElementType::Script);
	ZoneRenderManager->Init();
	ZoneRenderManager->ReadSource("Resource/Text/Map/Stage1_zone.txt");

#pragma endregion ZoneRenderManager
#pragma region TileRenderManager
	GameObject* TileRenderManagerObject = m_gameEngine->CreateObject("TileRenderManager", thisScene);
	Transform* TileRenderManagerTransform = new Transform();
	TileRenderManagerObject->SetElement(TileRenderManagerTransform, ElementType::Transform);
	TileRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	TileRenderManagerTransform->SetLayer(6);
	TileRenderManager* tileRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY,TileRenderState::Object);
	TileRenderManagerObject->SetElement(tileRenderManager, ElementType::Script);
	tileRenderManager->Init();
	tileRenderManager->ReadSource("Resource/Text/Map/Stage1_object.txt");
#pragma endregion TileRenderManager

#pragma region TileManager
	GameObject* TileManagerObject = m_gameEngine->CreateObject("TileManager", thisScene);
	Transform* TileManagerTransform = new Transform();
	TileManagerObject->SetElement(TileManagerTransform, ElementType::Transform);
	TileManagerTransform->SetUseCamera(true);
	TileManagerTransform->SetLayer(2);
	// 개별 타일의 오브젝트 및 콜라이더를 tileManager Script 안에서 생성함
	TileManager* tileManager = new TileManager(m_gameEngine, m_directY);
	TileManagerObject->SetElement(tileManager, ElementType::Script);
	tileManager->ReadSource2("Resource/Text/Map/Stage1_check.txt");
	tileManager->Init();
#pragma endregion TileManager

#pragma region Cloud
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud1", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ -1900.f, 700.f });
		cloudTransform->SetLayer(1000);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud1_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud2", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ 1900.f, 3200.f });
		cloudTransform->SetLayer(1000);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud2_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud3", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({-1900.f, 3200.f });
		cloudTransform->SetLayer(1000);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud1_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
#pragma endregion Cloud

#pragma region NpcManager
	GameObject* NpcManagerObject = m_gameEngine->CreateObject("NpcManager", thisScene);
	Transform* NpcManagerTransform = new Transform();
	NpcManagerObject->SetElement(NpcManagerTransform, ElementType::Transform);
	NpcManagerTransform->SetUseCamera(true);
	NpcManager* npcManager = new NpcManager(m_gameEngine, thisScene, m_directY);
	NpcManagerObject->SetElement(npcManager, ElementType::Script);
	npcManager->Init();
	npcManager->ReadSource("Resource/Text/Map/Stage1_npcLayer.txt");
	npcManager->SetPrintLog([=](std::wstring log, bool isNpc) { scrollbar->AddText(log,isNpc); });
	//npcManager->MakeNpc();
	npcManager->MakeObject();
#pragma endregion NpcManager

#pragma region CarNpcManager
	GameObject* CarNpcManagerObject = m_gameEngine->CreateObject("CarNpcManager", thisScene);
	Transform* CarNpcManagerTransform = new Transform();
	CarNpcManagerObject->SetElement(CarNpcManagerTransform, ElementType::Transform);
	CarNpcManagerTransform->SetUseCamera(true);
	NpcManager* CarnpcManager = new NpcManager(m_gameEngine, thisScene, m_directY, NpcType::Car);
	CarNpcManagerObject->SetElement(CarnpcManager, ElementType::Script);
	CarnpcManager->Init();
	CarnpcManager->ReadSource("Resource/Text/Map/Stage1_carLayer.txt");
	CarnpcManager->MakeObject();
#pragma endregion CarNpcManager

#pragma region NpcWaitManager
	///NPC대기열을 관리하는 오브젝트

	GameObject* npcWaitManagerObject = m_gameEngine->CreateObject("NpcWaitManager", thisScene);
	Transform* npcWaitManagerTransform = new Transform;
	npcWaitManagerObject->SetElement(npcWaitManagerTransform, ElementType::Transform);

	NPCWaitManager* npcWaitManager = new NPCWaitManager(npcManager);
	npcWaitManagerObject->SetElement(npcWaitManager, ElementType::Script);

	///면접자를 알아야한다
	heroFaceUI->SetInterviewList(npcWaitManager->GetWaitQueue());
#pragma endregion NpcWaitManager

#pragma region ClearDialogManager
	GameObject* clearDialogManagerObject = m_gameEngine->CreateObject("ClearDialogManager", thisScene);
	Transform* clearDialogManagerTransform = new Transform();
	clearDialogManagerObject->SetElement(clearDialogManagerTransform, ElementType::Transform);
	clearDialogManagerTransform->SetUseCamera(false);
	clearDialogManagerTransform->SetLayer(10000);

	Graphics::Sprite* clearDialogImage = m_directY->ReadSpriteTxtFile("dialog", m_gameEngine->GetImage("dialog"));
	clearDialogImage->m_drawPos = { 960 - dialogImage->m_drawSize.right / 2, 775 };

	Graphics::Sprite* clearDialogGodImage = m_directY->ReadSpriteTxtFile("dialogGod", m_gameEngine->GetImage("dialogGod"));
	clearDialogGodImage->m_drawPos = { 960 - dialogGodImage->m_drawSize.right / 2, 760 };

	DialogManager* clearDialogManager = new DialogManager(0, 0, 1396, 1080, 1100, clearDialogImage, clearDialogGodImage);
	clearDialogManagerObject->SetElement(clearDialogManager, ElementType::Script);
	clearDialogManager->ReadDialogCSV("Resource/Dialog2");
	clearDialogManager->SetTextOffset({ 130 + 960 - dialogGodImage->m_drawSize.right / 2, 860 });
	clearDialogManager->SetStartGame([=]()
		{
			m_directY->DeletePaticleEffect();
			m_gameEngine->SetNextScene("StageScene");
			ClearManager::GetInstance().SetClear(false);
			ClearManager::GetInstance().SetDead(false);

			ClearManager::GetInstance().SetClearStage(1);
		});

	AudioSource* clearDialogManagerAudioSource = new AudioSource(m_gameEngine);
	clearDialogManagerObject->SetElement(clearDialogManagerAudioSource, ElementType::AudioSource);
	clearDialogManager->SetAudioSource(clearDialogManagerAudioSource);
	clearDialogManagerAudioSource->AddSound("sfx_dialog_1");

	//BGI 로 변경
	Graphics::Sprite* cutScene1 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_1"));
	clearDialogManager->AddSprite("CutScene_1", cutScene1);
	Graphics::Sprite* cutScene2 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_2"));
	clearDialogManager->AddSprite("CutScene_2", cutScene2);

	clearDialogManagerObject->SetActive(false);
	DebugManager::GetInstance().SetObject(clearDialogManagerObject);
#pragma endregion ClearDialogManager

#pragma region MessageManger
	GameObject* messageManagerObject = m_gameEngine->CreateObject("messageManager", thisScene);
	Transform* messageManagerTransform = new Transform();
	messageManagerObject->SetElement(messageManagerTransform, ElementType::Transform);
	messageManagerTransform->SetUseCamera(true);
	messageManagerTransform->SetLayer(500);

	MessageManager* messageManager = new MessageManager(npcWaitManager, clearDialogManagerObject);
	messageManagerObject->SetElement(messageManager, ElementType::Script);
	messageManager->ReadMessageCSV("Resource/NPC_message_list");
	messageManager->ReadDialogCSV("Resource/NPC_message");
	messageManager->SetQueue(npcWaitManager->GetWaitQueue());
	messageManager->SetClearCount(2);
#pragma endregion MessageManger
	 
#pragma region Target
	GameObject* TargerBarObject = m_gameEngine->CreateObject("TargetBar", thisScene);
	Transform* TargerBarObjectTransform = new Transform();
	TargerBarObject->SetElement(TargerBarObjectTransform, ElementType::Transform);
	TargerBarObjectTransform->SetLocalPosition({ 10,10 });
	TargerBarObjectTransform->SetLayer(1000);

	TargetBar* targetBar = new TargetBar(messageManager,npcManager,npcWaitManager);
	TargerBarObject->SetElement(targetBar, ElementType::Script);

	Graphics::Motion* answerImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("AnswerList", m_gameEngine->GetImage("AnswerList"), answerImage);
	targetBar->SetImage(answerImage);

	Graphics::Motion* toneDownImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("AnswerList", m_gameEngine->GetImage("targettonedown"), toneDownImage);
	targetBar->SetTonedown(toneDownImage);

	Graphics::Motion* targetBackGround = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("targetbar_4", m_gameEngine->GetImage("targetbar_4"), targetBackGround);
	targetBar->SetBackGround(targetBackGround);

#pragma endregion Target
}

void GameProcess::InitGameScene2()
{
	Scene* thisScene = m_gameEngine->FindScene("GameScene2");
	m_gameEngine->LoadResource("GameScene", m_directY);

	m_gameEngine->SetTimeScale(0);

	/// 테스트용 bgm재생	
	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("bgm_ingame_02_loop"), 1.f, PlayType::BGM);

#pragma region DialogManager
	GameObject* dialogManagerObject = m_gameEngine->CreateObject("DialogManager", thisScene);
	Transform* dialogManagerTransform = new Transform();
	dialogManagerObject->SetElement(dialogManagerTransform, ElementType::Transform);
	dialogManagerTransform->SetUseCamera(false);
	dialogManagerTransform->SetLayer(10000);

	Graphics::Sprite* dialogImage = m_directY->ReadSpriteTxtFile("dialog", m_gameEngine->GetImage("dialog"));
	dialogImage->m_drawPos = { 37, 775 };

	Graphics::Sprite* dialogGodImage = m_directY->ReadSpriteTxtFile("dialogGod", m_gameEngine->GetImage("dialogGod"));
	dialogGodImage->m_drawPos = { 30, 760 };

	DialogManager* dialogManager = new DialogManager(0, 0, 1396, 1080, 2000, dialogImage, dialogGodImage);
	dialogManagerObject->SetElement(dialogManager, ElementType::Script);
	dialogManager->ReadDialogCSV("Resource/Dialog2");
	dialogManager->SetTextOffset({ 130, 860 });

	Graphics::Sprite* goddessImageSprite = m_directY->ReadSpriteTxtFile("goddessStanding", m_gameEngine->GetImage("goddess"));
	Graphics::Sprite* goddessDarkImageSprite = m_directY->ReadSpriteTxtFile("goddessStanding", m_gameEngine->GetImage("goddess_Dark"));
	Graphics::Sprite* playerImageSprite = m_directY->ReadSpriteTxtFile("playerStanding", m_gameEngine->GetImage("Player"));
	Graphics::Sprite* playerDarkImageSprite = m_directY->ReadSpriteTxtFile("playerStanding", m_gameEngine->GetImage("Player_Dark"));

	dialogManager->AddSprite("goddess", goddessImageSprite);
	dialogManager->AddSprite("goddess_Dark", goddessDarkImageSprite);
	dialogManager->AddSprite("Player", playerImageSprite);
	dialogManager->AddSprite("Player_Dark", playerDarkImageSprite);

	AudioSource* dialogManagerAudioSource = new AudioSource(m_gameEngine);
	dialogManagerObject->SetElement(dialogManagerAudioSource, ElementType::AudioSource);
	dialogManager->SetAudioSource(dialogManagerAudioSource);
	dialogManagerAudioSource->AddSound("sfx_dialog_1");

#pragma endregion DialogManager

#pragma region UI

#pragma region MainUI
	/// MainUIWindow
	GameObject* UIObject = m_gameEngine->CreateObject("UI", thisScene);
	Transform* uiTransform = new Transform();
	UIObject->SetElement(uiTransform, ElementType::Transform);
	uiTransform->SetLocalPosition({ 1398,40 });
	uiTransform->SetLayer(300);

	InGameUI* mainUI = new InGameUI();
	UIObject->SetElement(mainUI, ElementType::Script);
	Graphics::Sprite* UIImage = m_directY->ReadSpriteTxtFile("ingame_mainsolid", m_gameEngine->GetImage("ingame_mainsolid"));
	mainUI->m_sprite = UIImage;
#pragma endregion MainUI

#pragma region HeroUI
	///면접자 초상화UI
	GameObject* heroPotraitObject = m_gameEngine->CreateObject("heroPotrait", thisScene);
	Transform* heroPotraitTransform = new Transform();
	heroPotraitObject->SetElement(heroPotraitTransform, ElementType::Transform);
	heroPotraitTransform->SetParent(uiTransform);
	heroPotraitTransform->Translate({ 25,446 });
	heroPotraitTransform->SetLayer(312);

	InterviewUI* heroPotraitUI = new InterviewUI();
	heroPotraitObject->SetElement(heroPotraitUI, ElementType::Script);
	Graphics::Motion* interviewImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("heroPotrait", m_gameEngine->GetImage("heroPotrait"), interviewImage);
	heroPotraitUI->SetImage(interviewImage);

	///면접자 얼굴 이미지
	GameObject* heroFaceObject = m_gameEngine->CreateObject("heroPotrait", thisScene);
	Transform* heroFaceTransform = new Transform();
	heroFaceObject->SetElement(heroFaceTransform, ElementType::Transform);
	heroFaceTransform->SetParent(uiTransform);
	heroFaceTransform->Translate({ 35,446 });
	heroFaceTransform->SetLayer(313);

	PotraitUI* heroFaceUI = new PotraitUI();
	heroFaceObject->SetElement(heroFaceUI, ElementType::Script);
	Graphics::Motion* potraitImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("potraitGroup", m_gameEngine->GetImage("potraitGroup"), potraitImage);
	heroFaceUI->SetPotraitGroup(potraitImage);

	///면접자 초상화 프레임
	GameObject* heroFrameObject = m_gameEngine->CreateObject("heroframe", thisScene);
	Transform* heroFrameTransform = new Transform();
	heroFrameObject->SetElement(heroFrameTransform, ElementType::Transform);
	heroFrameTransform->SetParent(uiTransform);
	heroFrameTransform->SetLocalPosition({ 25,446 });
	heroFrameTransform->SetLayer(314);

	InGameUI* heroFrameUI = new InGameUI();
	heroFrameObject->SetElement(heroFrameUI, ElementType::Script);
	Graphics::Sprite* heroFrameImage = m_directY->ReadSpriteTxtFile("heroframe", m_gameEngine->GetImage("heroframe"));
	heroFrameUI->m_sprite = heroFrameImage;
#pragma endregion HeroUI

#pragma region GoddessUI
	///여신 초상화
	GameObject* goddessPotraitObject = m_gameEngine->CreateObject("goddessPotrait", thisScene);
	Transform* goddessPotraitTransform = new Transform();
	goddessPotraitObject->SetElement(goddessPotraitTransform, ElementType::Transform);
	goddessPotraitTransform->SetParent(uiTransform);
	goddessPotraitTransform->Translate({ 305,446 });
	goddessPotraitTransform->SetLayer(312);

	InterviewUI* goddesspotraitUI = new InterviewUI();
	goddessPotraitObject->SetElement(goddesspotraitUI, ElementType::Script);
	Graphics::Motion* goddessImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("goddessPotrait", m_gameEngine->GetImage("goddessPotrait"), goddessImage);
	goddesspotraitUI->SetImage(goddessImage);

	///여신 얼굴 이미지
	GameObject* goddessFaceObject = m_gameEngine->CreateObject("goddessPotrait", thisScene);
	Transform* goddessFaceTransform = new Transform();
	goddessFaceObject->SetElement(goddessFaceTransform, ElementType::Transform);
	goddessFaceTransform->SetParent(uiTransform);
	goddessFaceTransform->Translate({ 315,453 });
	goddessFaceTransform->SetLayer(313);

	InterviewUI* goddessFaceUI = new InterviewUI();
	goddessFaceObject->SetElement(goddessFaceUI, ElementType::Script);
	Graphics::Motion* goddessFaceImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("portrait", m_gameEngine->GetImage("portrait_godness"), goddessFaceImage);
	goddessFaceUI->SetImage(goddessFaceImage);

	///여신 초상화 프레임
	GameObject* goddessBarFrameObject = m_gameEngine->CreateObject("goddessbarframe", thisScene);
	Transform* goddessBarFrameTransform = new Transform();
	goddessBarFrameObject->SetElement(goddessBarFrameTransform, ElementType::Transform);
	goddessBarFrameTransform->SetParent(uiTransform);
	goddessBarFrameTransform->Translate({ 305,446 });
	goddessBarFrameTransform->SetLayer(314);

	InterviewUI* goddessBarFrameUI = new InterviewUI();
	goddessBarFrameObject->SetElement(goddessBarFrameUI, ElementType::Script);
	Graphics::Motion* goddessBarFrameImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("goddessbarframe", m_gameEngine->GetImage("goddessbarframe"), goddessBarFrameImage);
	goddessBarFrameUI->SetImage(goddessBarFrameImage);
#pragma endregion GoddessUI

#pragma region TimerUI
	///Timer
	GameObject* TimerObject = m_gameEngine->CreateObject("Timer", thisScene);
	Transform* TimerTransform = new Transform();
	TimerObject->SetElement(TimerTransform, ElementType::Transform);
	// 부모트랜스폼 설정
	// 트랜스폼 레이어 설정(일단 보일 수 있도록 높은 숫자 넣음)
	TimerTransform->SetLayer(314);
	Timer* timer = new Timer(m_gameEngine, m_directY);
	TimerObject->SetElement(timer, ElementType::Script);

	// 오디오 소스를 추가함
	AudioSource* timerAudioSource = new AudioSource(m_gameEngine);
	TimerObject->SetElement(timerAudioSource, ElementType::AudioSource);
	timer->SetAudioSource(timerAudioSource);
	timerAudioSource->AddSound("bgm_beep_slow");
	timerAudioSource->AddSound("sfx_timer_warning");

	///타이머 장식
	GameObject* secondBaseObject = m_gameEngine->CreateObject("secondHandBase", thisScene);
	Transform* secondBaseTransform = new Transform();
	secondBaseTransform->SetParent(uiTransform);
	secondBaseTransform->SetLocalPosition({ 205 , 622 });
	secondBaseObject->SetElement(secondBaseTransform, ElementType::Transform);
	secondBaseTransform->SetLayer(315);

	InGameUI* secondBaseUI = new InGameUI();
	secondBaseObject->SetElement(secondBaseUI, ElementType::Script);
	Graphics::Sprite* secondBaseImage = m_directY->ReadSpriteTxtFile("secondhand_base", m_gameEngine->GetImage("secondhand_base"));
	secondBaseUI->m_sprite = secondBaseImage;

	///타이머 프레임
	GameObject* clockbaseObject = m_gameEngine->CreateObject("clockbase", thisScene);
	Transform* clockbaseTransform = new Transform();
	clockbaseTransform->SetParent(uiTransform);
	clockbaseTransform->SetLocalPosition({ 120 , 652 });
	clockbaseObject->SetElement(clockbaseTransform, ElementType::Transform);
	clockbaseTransform->SetLayer(314);

	InGameUI* clockbaseUI = new InGameUI();
	clockbaseObject->SetElement(clockbaseUI, ElementType::Script);
	Graphics::Sprite* clockbaseImage = m_directY->ReadSpriteTxtFile("clockbase", m_gameEngine->GetImage("clockbase"));
	clockbaseUI->m_sprite = clockbaseImage;
	dialogManager->SetStartGame([=]()
		{
			m_gameEngine->SetTimeScale(1);
			timer->StartTimer();
		});
#pragma endregion TimerUI

#pragma region MessageUI
	/// 메세지 창 Scrollbar
	GameObject* dialogueObject = m_gameEngine->CreateObject("Scrollbar", thisScene);
	Transform* dialogueTrasform = new Transform();
	dialogueObject->SetElement(dialogueTrasform, ElementType::Transform);
	dialogueTrasform->SetParent(uiTransform);
	dialogueTrasform->Translate({ 25, 35 });
	dialogueTrasform->SetLayer(310);

	Graphics::Sprite* bar = m_directY->ReadSpriteTxtFile("bar", m_gameEngine->GetImage("bar"));
	Graphics::Sprite* background = m_directY->ReadSpriteTxtFile("message", m_gameEngine->GetImage("message"));
	Graphics::Motion* herochatbubble = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("herochatwindow", m_gameEngine->GetImage("chatwindow"), herochatbubble);

	Graphics::Motion* goddesschatbubble = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("herochatwindow", m_gameEngine->GetImage("chatwindow_goddess"), goddesschatbubble);

	Scrollbar* scrollbar = new Scrollbar(35, 50, 422 - 35, 380 - 50, background, bar, herochatbubble, goddesschatbubble);
	dialogueObject->SetElement(scrollbar, ElementType::Scrollbar);
	scrollbar->SetHeroChatWindow(herochatbubble);
	scrollbar->SetGoddessChatWindow(goddesschatbubble);

	/*///메세지 프레임
	GameObject* messageFrameObject = m_gameEngine->CreateObject("heroframe", thisScene);
	Transform* messageFrameTransform = new Transform();
	messageFrameObject->SetElement(messageFrameTransform, ElementType::Transform);
	messageFrameTransform->SetParent(uiTransform);
	messageFrameTransform->SetLocalPosition({ 25,35 });
	messageFrameTransform->SetLayer(312);

	InGameUI* messageFrameUI = new InGameUI();
	messageFrameObject->SetElement(messageFrameUI, ElementType::Script);
	Graphics::Sprite* messageFrameImage = m_directY->ReadSpriteTxtFile("messageframe", m_gameEngine->GetImage("messageframe"));
	messageFrameUI->m_sprite = messageFrameImage;*/
#pragma endregion MessageUI

#pragma region InterviewRoomUI
	///면접장
	GameObject* InterviewRoomObject = m_gameEngine->CreateObject("interviewPlace", thisScene);
	Transform* InterviewRoomTransform = new Transform();
	InterviewRoomTransform->SetParent(uiTransform);
	InterviewRoomTransform->SetLocalPosition({ 36, 672 });
	InterviewRoomObject->SetElement(InterviewRoomTransform, ElementType::Transform);
	InterviewRoomTransform->SetLayer(310);

	InterviewUI* InterviewRoomUI = new InterviewUI();
	InterviewRoomObject->SetElement(InterviewRoomUI, ElementType::Script);
	Graphics::Motion* interviewPlaceImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_bar", m_gameEngine->GetImage("interview_screen_bar"), interviewPlaceImage);
	InterviewRoomUI->SetImage(interviewPlaceImage);

	/// 면접장 배경
	GameObject* InterviewRoomBGObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewRoomBGTransform = new Transform();
	InterviewRoomBGTransform->SetParent(uiTransform);
	InterviewRoomBGTransform->SetLocalPosition({ 35, 673 });
	InterviewRoomBGObject->SetElement(InterviewRoomBGTransform, ElementType::Transform);
	InterviewRoomBGTransform->SetLayer(311);

	InterviewUI* InterviewRoomBGUI = new InterviewUI();
	InterviewRoomBGObject->SetElement(InterviewRoomBGUI, ElementType::Script);
	Graphics::Motion* interviewRoomImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("InterviewRoomBackground", m_gameEngine->GetImage("InterviewRoomBackground"), interviewRoomImage);
	InterviewRoomBGUI->SetImage(interviewRoomImage);

	///면접장 빛
	GameObject* InterviewLightObject = m_gameEngine->CreateObject("interviewLight", thisScene);
	Transform* InterviewLightTransform = new Transform();
	InterviewLightTransform->SetParent(uiTransform);
	InterviewLightTransform->SetLocalPosition({ 35, 673 });
	InterviewLightObject->SetElement(InterviewLightTransform, ElementType::Transform);
	InterviewLightTransform->SetLayer(312);

	InterviewUI* InterviewLightUI = new InterviewUI();
	InterviewLightObject->SetElement(InterviewLightUI, ElementType::Script);
	Graphics::Motion* interviewLightImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interviewLight", m_gameEngine->GetImage("interviewLight"), interviewLightImage);
	InterviewLightUI->SetImage(interviewLightImage);

	/// 면접장 프레임
	GameObject* InterviewFrameObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewFrameTransform = new Transform();
	InterviewFrameTransform->SetParent(uiTransform);
	InterviewFrameTransform->SetLocalPosition({ 25, 535 });
	InterviewFrameObject->SetElement(InterviewFrameTransform, ElementType::Transform);
	InterviewFrameTransform->SetLayer(312);

	InterviewUI* InterviewFrameUI = new InterviewUI();
	InterviewFrameObject->SetElement(InterviewFrameUI, ElementType::Script);
	Graphics::Motion* interviewFrameImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_frame", m_gameEngine->GetImage("interview_screen_frame"), interviewFrameImage);
	InterviewFrameUI->SetImage(interviewFrameImage);

	///면접장 데코
	GameObject* InterviewDecoObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewDecoTransform = new Transform();
	InterviewDecoTransform->SetParent(uiTransform);
	InterviewDecoTransform->SetLocalPosition({ 20, 652 });
	InterviewDecoObject->SetElement(InterviewDecoTransform, ElementType::Transform);
	InterviewDecoTransform->SetLayer(313);

	InterviewUI* InterviewDecoUI = new InterviewUI();
	InterviewDecoObject->SetElement(InterviewDecoUI, ElementType::Script);
	Graphics::Motion* interviewDecoImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_frame_deco", m_gameEngine->GetImage("interview_screen_frame_deco"), interviewDecoImage);
	InterviewDecoUI->SetImage(interviewDecoImage);
#pragma endregion InterviewRoomUI

#pragma  region MainOptionUI

	GameObject* MainOptionUIObject = m_gameEngine->CreateObject("MainOptionUI", thisScene);
	Transform* MainOptionUITransform = new Transform;
	MainOptionUIObject->SetElement(MainOptionUITransform, ElementType::Transform);
	MainOptionUITransform->SetLayer(500);
	MainOptionUITransform->SetLocalPosition({ 960 - 315,540 - 215 });
	MainOptionUIObject->SetActive(false);

	MainOption* mainOptionUI = new MainOption();
	MainOptionUIObject->SetElement(mainOptionUI, ElementType::Script);
	Graphics::Sprite* mainOptionImage = m_directY->ReadSpriteTxtFile("menubar_all", m_gameEngine->GetImage("menubar_all"));
	mainOptionUI->SetImage(mainOptionImage);

	/// 타이틀로 가기 옵션 버튼
	GameObject* mainButtonObject = m_gameEngine->CreateObject("MainTitleButton", thisScene);
	mainButtonObject->SetActive(false);

	Transform* mainButtonTransform = new Transform();
	mainButtonObject->SetElement(mainButtonTransform, ElementType::Transform);
	mainButtonTransform->SetParent(MainOptionUITransform);
	mainButtonTransform->SetLocalPosition({ 185, 125 });

	Vector2 mainButtonWorldPosition = mainButtonTransform->GetWorldPosition();
	Button* mainButton = new Button(0 + (int)mainButtonWorldPosition.x, 0 + (int)mainButtonWorldPosition.y, 266 + (int)mainButtonWorldPosition.x, 71 + (int)mainButtonWorldPosition.y);
	mainButtonObject->SetElement(mainButton, ElementType::Script);
	mainButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("TitleScene");
			}
		});

	// 메인버튼 오디오소스
	AudioSource* mainButtonAudioSource = new AudioSource(m_gameEngine);
	mainButtonObject->SetElement(mainButtonAudioSource, ElementType::AudioSource);
	mainButton->SetAudioSource(mainButtonAudioSource);
	mainButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	/// 다시하기 옵션 버튼
	GameObject* replayButtonObject = m_gameEngine->CreateObject("ReplayButton", thisScene);
	replayButtonObject->SetActive(false);

	Transform* replayButtonTransform = new Transform();
	replayButtonObject->SetElement(replayButtonTransform, ElementType::Transform);
	replayButtonTransform->SetParent(MainOptionUITransform);
	replayButtonTransform->SetLocalPosition({ 185, 215 });

	Vector2 replaybuttonWorldPosition = replayButtonTransform->GetWorldPosition();
	Button* replayButton = new Button(0 + (int)replaybuttonWorldPosition.x, 0 + (int)replaybuttonWorldPosition.y, 266 + (int)replaybuttonWorldPosition.x, 71 + (int)replaybuttonWorldPosition.y);
	replayButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("GameScene2");
			}
		});
	replayButtonObject->SetElement(replayButton, ElementType::Script);

	// 다시하기 버튼 오디오소스
	AudioSource* replayButtonAudioSource = new AudioSource(m_gameEngine);
	replayButtonObject->SetElement(replayButtonAudioSource, ElementType::AudioSource);
	replayButton->SetAudioSource(replayButtonAudioSource);
	replayButtonAudioSource->AddSoundVector("sfx_OnButton");
	replayButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	///옵션 닫기
	GameObject* exitButtonObject = m_gameEngine->CreateObject("exitbutton", thisScene);

	Transform* exitbuttonTransform = new Transform();
	exitButtonObject->SetElement(exitbuttonTransform, ElementType::Transform);
	exitbuttonTransform->SetLayer(501);
	exitButtonObject->SetActive(false);

	Button* exitButton = new Button(1240, 325, 1276, 360);
	Graphics::Sprite* exiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	exitButton->m_sprite = exiteButtonImage;
	exitButtonObject->SetElement(exitButton, ElementType::Button);

	AudioSource* exitButtonAudioSource = new AudioSource(m_gameEngine);
	exitButtonObject->SetElement(exitButtonAudioSource, ElementType::AudioSource);
	exitButton->SetAudioSource(exitButtonAudioSource);
	exitButtonAudioSource->AddSoundVector("sfx_OnButton");
	exitButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	/// 우상단 옵션 버튼
	GameObject* MainButtonObject = m_gameEngine->CreateObject("mainbutton", thisScene);
	Transform* MainButtonTransform = new Transform();
	MainButtonObject->SetElement(MainButtonTransform, ElementType::Transform);
	MainButtonTransform->SetLayer(500);

	Button* MainOptionButton = new Button(1865, 15, 1905, 55);
	Graphics::Sprite* MainButtonImage = m_directY->ReadSpriteTxtFile("button", m_gameEngine->GetImage("menubutton"));
	MainOptionButton->m_sprite = MainButtonImage;
	MainButtonObject->SetElement(MainOptionButton, ElementType::Button);

	AudioSource* mainOptionButtonAudioSource = new AudioSource(m_gameEngine);
	MainButtonObject->SetElement(mainOptionButtonAudioSource, ElementType::AudioSource);
	MainOptionButton->SetAudioSource(mainOptionButtonAudioSource);
	mainOptionButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainOptionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion MainOptionUI	

#pragma region SoundOptionUI
	/// 옵션UI생성
	/// OptionUIObject 생성
	GameObject* SoundOptionUIObject = m_gameEngine->CreateObject("optionUI", thisScene);
	Transform* soundOptionUITransform = new Transform;
	SoundOptionUIObject->SetElement(soundOptionUITransform, ElementType::Transform);
	soundOptionUITransform->SetLayer(510);
	soundOptionUITransform->SetLocalPosition({ (1920 / 2) - 315.5f ,(1080 / 2) - 215.5f });

	// 볼륨슬라이더 오브젝트 생성
	GameObject* MasterVolumeSliderObject = m_gameEngine->CreateObject("MsterVolumeSlider", thisScene);
	GameObject* BGMVolumeSliderObject = m_gameEngine->CreateObject("BGMVolumeSlider", thisScene);
	GameObject* SFXVolumeSliderObject = m_gameEngine->CreateObject("SFXVolumeSlider", thisScene);

	// 슬라이더 트랜스폼
	Transform* masterVolumeSliderTransform = new Transform;
	Transform* BGMVolumeSliderTransform = new Transform;
	Transform* SFXVolumeSliderTransform = new Transform;

	MasterVolumeSliderObject->SetElement(masterVolumeSliderTransform, ElementType::Transform);
	BGMVolumeSliderObject->SetElement(BGMVolumeSliderTransform, ElementType::Transform);
	SFXVolumeSliderObject->SetElement(SFXVolumeSliderTransform, ElementType::Transform);

	masterVolumeSliderTransform->SetParent(soundOptionUITransform);
	BGMVolumeSliderTransform->SetParent(soundOptionUITransform);
	SFXVolumeSliderTransform->SetParent(soundOptionUITransform);

	masterVolumeSliderTransform->SetLayer(511);
	BGMVolumeSliderTransform->SetLayer(511);
	SFXVolumeSliderTransform->SetLayer(511);

	// 위치를 미리 찍는다.
	masterVolumeSliderTransform->SetLocalPosition({ 160, 100 });
	BGMVolumeSliderTransform->SetLocalPosition({ 160, 200 });
	SFXVolumeSliderTransform->SetLocalPosition({ 160, 300 });

	// 볼륨슬라이더 리소스 생성
	// (슬라이더 리소스들의 해제가 이루어지지 않고 있음)
	// ui
	Graphics::Sprite* soundOptionUIBackground = m_directY->ReadSpriteTxtFile("sounduibar", m_gameEngine->GetImage("sounduibar"));
	// slider
	Graphics::Sprite* sliderBackground_Master = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_BGM = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_SFX = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderFillArea_Master = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_Master = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_BGM = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_BGM = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_SFX = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_SFX = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));

	Graphics::Sprite* BGImage = m_directY->ReadSpriteTxtFile("bgsound", m_gameEngine->GetImage("bgsound")); //배경음 글자 이미지
	Graphics::Sprite* EffectImage = m_directY->ReadSpriteTxtFile("effectsound", m_gameEngine->GetImage("effectsound")); //효과음 글자 이미지
	Graphics::Sprite* MasterImage = m_directY->ReadSpriteTxtFile("mastersound", m_gameEngine->GetImage("mastersound")); //마스터 글자 이미지

	// 슬라이더 AA
	Slider* masterVolumeSlider = new Slider((int)sliderBackground_Master->m_drawSize.left, (int)sliderBackground_Master->m_drawSize.right,
		(int)sliderBackground_Master->m_drawSize.top, (int)sliderBackground_Master->m_drawSize.bottom,
		(int)(sliderBackground_Master->m_drawSize.left + sliderBackground_Master->m_drawSize.right) / 2,
		(int)(sliderBackground_Master->m_drawSize.top, sliderBackground_Master->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(MASTER));
	Slider* SFXVolumeSlider = new Slider((int)sliderBackground_SFX->m_drawSize.left, (int)sliderBackground_SFX->m_drawSize.right,
		(int)sliderBackground_SFX->m_drawSize.top, (int)sliderBackground_SFX->m_drawSize.bottom,
		(int)(sliderBackground_SFX->m_drawSize.left + sliderBackground_SFX->m_drawSize.right) / 2,
		(int)(sliderBackground_SFX->m_drawSize.top, sliderBackground_SFX->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(SFX));
	Slider* BGMVolumeSlider = new Slider((int)sliderBackground_BGM->m_drawSize.left, (int)sliderBackground_BGM->m_drawSize.right,
		(int)sliderBackground_BGM->m_drawSize.top, (int)sliderBackground_BGM->m_drawSize.bottom,
		(int)(sliderBackground_BGM->m_drawSize.left + sliderBackground_BGM->m_drawSize.right) / 2,
		(int)(sliderBackground_BGM->m_drawSize.top, sliderBackground_BGM->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(BGM));

	// 슬라이더 이미지 저장
	masterVolumeSlider->SetBackground(sliderBackground_Master);
	masterVolumeSlider->SetFillArea(sliderFillArea_Master);
	masterVolumeSlider->SetHandle(sliderBar_Master);

	BGMVolumeSlider->SetBackground(sliderBackground_BGM);
	BGMVolumeSlider->SetFillArea(sliderFillArea_BGM);
	BGMVolumeSlider->SetHandle(sliderBar_BGM);

	SFXVolumeSlider->SetBackground(sliderBackground_SFX);
	SFXVolumeSlider->SetFillArea(sliderFillArea_SFX);
	SFXVolumeSlider->SetHandle(sliderBar_SFX);

	MasterVolumeSliderObject->SetElement(masterVolumeSlider, ElementType::Slider);
	BGMVolumeSliderObject->SetElement(BGMVolumeSlider, ElementType::Slider);
	SFXVolumeSliderObject->SetElement(SFXVolumeSlider, ElementType::Slider);

	// 슬라이더 오브젝트만 가져서 수치 반영만 한다.
	SoundOptionUI* soundOptionUI = new SoundOptionUI(masterVolumeSlider, BGMVolumeSlider, SFXVolumeSlider);
	SoundOptionUIObject->SetElement(soundOptionUI, ElementType::Script);
	soundOptionUI->SetUISprite(soundOptionUIBackground);
	soundOptionUI->SetMasterTiltle(MasterImage);
	soundOptionUI->SetBgmTiltle(BGImage);
	soundOptionUI->SetSfxTiltle(EffectImage);

	soundOptionUI->Init();

	/// 사운드 옵션 끄기 버튼
	GameObject* soundExitButtonObject = m_gameEngine->CreateObject("soundExitbutton", thisScene);
	Transform* soundExitbuttonTransform = new Transform();
	soundExitButtonObject->SetElement(soundExitbuttonTransform, ElementType::Transform);
	soundExitbuttonTransform->SetLocalPosition({ 597, 0 });
	soundExitbuttonTransform->SetLayer(511);
	soundExitButtonObject->SetActive(false);
	soundExitButtonObject->SetIsDraw(false);

	Button* soundExitButton = new Button(1240, 325, 1275, 360);
	Graphics::Sprite* soundExiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	soundExitButton->m_sprite = soundExiteButtonImage;
	soundExitButton->SetOnClick([=]()
		{
			if (SoundOptionUIObject->GetActive())
			{
				MainOptionUIObject->SetActive(true);

				soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
				exitButtonObject->SetActive(!exitButtonObject->GetActive());
				SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetActive());
				MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
				SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
				BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
			}
		});
	soundExitButtonObject->SetElement(soundExitButton, ElementType::Button);

	AudioSource* soundExitButtonAudiosource = new AudioSource(m_gameEngine);
	soundExitButtonObject->SetElement(soundExitButtonAudiosource, ElementType::AudioSource);
	soundExitButton->SetAudioSource(soundExitButtonAudiosource);
	soundExitButtonAudiosource->AddSoundVector("sfx_OnButton");
	soundExitButtonAudiosource->AddSoundVector("sfx_StageSelect_click");

	/// 음량 옵션 버튼
	GameObject* ButtonObject = m_gameEngine->CreateObject("soundbutton", thisScene);
	ButtonObject->SetActive(false);
	Transform* buttonTransform = new Transform();
	ButtonObject->SetElement(buttonTransform, ElementType::Transform);
	buttonTransform->SetParent(MainOptionUITransform);
	buttonTransform->SetLocalPosition({ 185, 310 });
	buttonTransform->SetLayer(501);
	Button* soundOptionButton = new Button(830, 635, 1095, 705);
	//Graphics::Sprite* buttonImage = m_directY->ReadSpriteTxtFile("soundbutton", m_gameEngine->GetImage("soundbutton"));
	//soundOptionButton->m_sprite = buttonImage;
	soundOptionButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				MainOptionUIObject->SetActive(false);

				exitButtonObject->SetActive(!exitButtonObject->GetActive());
				soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
				SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetIsDraw());
				MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
				SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
				BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
			}
		});
	ButtonObject->SetElement(soundOptionButton, ElementType::Button);

	AudioSource* soundOptionButtonAudioSource = new AudioSource(m_gameEngine);
	ButtonObject->SetElement(soundOptionButtonAudioSource, ElementType::AudioSource);
	soundOptionButton->SetAudioSource(soundOptionButtonAudioSource);
	soundOptionButtonAudioSource->AddSoundVector("sfx_OnButton");
	soundOptionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion SoundOptionUI

#pragma endregion OnClick
	MainOptionButton->SetOnClick([=]()
		{
			MainOptionUIObject->SetActive(true);
			exitButtonObject->SetActive(true);

			mainButtonObject->SetActive(true);
			replayButtonObject->SetActive(true);
			exitButtonObject->SetActive(true);
			ButtonObject->SetActive(true);
		});
	exitButton->SetOnClick([=]()
		{
			MainOptionUIObject->SetActive(false);
			exitButtonObject->SetActive(false);

			mainButtonObject->SetActive(false);
			replayButtonObject->SetActive(false);
			exitButtonObject->SetActive(false);
			ButtonObject->SetActive(false);
		});
#pragma endregion OnClick
#pragma endregion UI

#pragma region Player
	GameObject* PlayerObject = m_gameEngine->CreateObject("Player", thisScene);

	Transform* playerTransform = new Transform();
	PlayerObject->SetElement(playerTransform, ElementType::Transform);
	playerTransform->SetUseCamera(true);
	playerTransform->SetLayer(5);
	playerTransform->SetLocalPosition({ 432.f, 1080.f });

	Player* player = new Player();
	PlayerObject->SetElement(player, ElementType::Script);
	player->SetMyTransform(playerTransform);

	Graphics::Motion* truckMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("isekaiTruck", m_gameEngine->GetImage("Truck"), truckMotion);
	player->SetMotion(truckMotion);

	player->SetMyPaticle(m_directY->ReadSpriteTxtFile("starB", m_gameEngine->GetImage("starB")));
	player->SetMyPaticle2(m_directY->ReadSpriteTxtFile("starM", m_gameEngine->GetImage("starM")));

	for (int i = 0; i < 32; i++)
	{
		truckMotion->GetSprite(i)->m_drawPos.x -= truckMotion->GetSprite(i)->m_pivot.x;
		truckMotion->GetSprite(i)->m_drawPos.y -= truckMotion->GetSprite(i)->m_pivot.y;
	}

	Collider* playerCollider = m_gameEngine->GetCollisionManager()->CreateVehicleCollider(0.f, 0.f, 100, 200.f);
	m_gameEngine->GetCollisionManager()->SetPlayerCollider(playerCollider);
	m_gameEngine->GetCollisionManager()->SetPlayerObject(PlayerObject);
	PlayerObject->SetElement(playerCollider, ElementType::Collider);

	AudioSource* playerAudioScource = new AudioSource(m_gameEngine);
	PlayerObject->SetElement(playerAudioScource, ElementType::AudioSource);
	player->SetAudioSource(playerAudioScource);
	playerAudioScource->AddSound("sfx_hit_car_1");
	playerAudioScource->AddSound("sfx_hit_car_1");
	playerAudioScource->AddSound("sfx_hit_npc_1");
	playerAudioScource->AddSound("sfx_hit_npc_1");
	playerAudioScource->AddSound("sfx_hit_wall_1");
	playerAudioScource->AddSound("sfx_truck_start");
	playerAudioScource->AddSound("sfx_engineSoundMId_loop");
	playerAudioScource->AddSound("sfx_engineSoundBase_loop");
	playerAudioScource->AddSound("sfx_trucBack_loop");

#pragma endregion Player

#pragma region GroundRenderManager
	GameObject* GroundRenderManagerObject = m_gameEngine->CreateObject("GroundRenderManager", thisScene);
	Transform* GroundRenderManagerTransform = new Transform();
	GroundRenderManagerObject->SetElement(GroundRenderManagerTransform, ElementType::Transform);
	GroundRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	GroundRenderManagerTransform->SetLayer(0);
	TileRenderManager* groundRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY);
	GroundRenderManagerObject->SetElement(groundRenderManager, ElementType::Script);
	groundRenderManager->Init();
	groundRenderManager->ReadSource("Resource/Text/Map/Stage2_tile.txt");

#pragma endregion GroundRenderManager

#pragma region ZoneRenderManager
	GameObject* ZoneRenderManagerObject = m_gameEngine->CreateObject("ZoneRenderManager", thisScene);
	Transform* ZoneRenderManagerTransform = new Transform();
	ZoneRenderManagerObject->SetElement(ZoneRenderManagerTransform, ElementType::Transform);
	ZoneRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	ZoneRenderManagerTransform->SetLayer(6);
	TileRenderManager* zoneRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY);
	ZoneRenderManagerObject->SetElement(zoneRenderManager, ElementType::Script);
	zoneRenderManager->Init();
	zoneRenderManager->ReadSource("Resource/Text/Map/Stage2_zone.txt");
#pragma endregion ZoneRenderManager

#pragma region TileRenderManager
	GameObject* TileRenderManagerObject = m_gameEngine->CreateObject("TileRenderManager", thisScene);
	Transform* TileRenderManagerTransform = new Transform();
	TileRenderManagerObject->SetElement(TileRenderManagerTransform, ElementType::Transform);
	TileRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	TileRenderManagerTransform->SetLayer(6);
	TileRenderManager* tileRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY, TileRenderState::Object);
	TileRenderManagerObject->SetElement(tileRenderManager, ElementType::Script);
	tileRenderManager->Init();
	tileRenderManager->ReadSource("Resource/Text/Map/Stage2_object.txt");
#pragma endregion TileRenderManager

#pragma region TileManager
	GameObject* TileManagerObject = m_gameEngine->CreateObject("TileManager", thisScene);
	Transform* TileManagerTransform = new Transform();
	TileManagerObject->SetElement(TileManagerTransform, ElementType::Transform);
	TileManagerTransform->SetUseCamera(true);
	TileManagerTransform->SetLayer(2);
	// 개별 타일의 오브젝트 및 콜라이더를 tileManager Script 안에서 생성함
	TileManager* tileManager = new TileManager(m_gameEngine, m_directY);
	TileManagerObject->SetElement(tileManager, ElementType::Script);
	tileManager->ReadSource2("Resource/Text/Map/Stage2_check.txt");
	tileManager->Init();
#pragma endregion TileManager

#pragma region Cloud
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud1", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ -1900.f, 700.f });
		cloudTransform->SetLayer(300);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud1_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud2", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ 1900.f, 3200.f });
		cloudTransform->SetLayer(300);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud2_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud3", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ -1900.f, 3200.f });
		cloudTransform->SetLayer(300);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud1_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud4", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ 1900.f, 700.f });
		cloudTransform->SetLayer(300);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud1_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
#pragma endregion Cloud

#pragma region Police
	GameObject* policeObejct = m_gameEngine->CreateObject("Police", thisScene);
	Transform* policeTrasnform = new Transform();
	policeObejct->SetElement(policeTrasnform, ElementType::Transform);
	policeTrasnform->SetUseCamera(true);
	policeTrasnform->SetLayer(8);
	policeTrasnform->SetLocalPosition({ 864.f, 1224.f });

	Police* police = new Police(playerTransform, policeTrasnform, policeTrasnform->GetLocalPosition(), 
		"Resource/Text/Map/Stage2_check.txt", { {11, 21}, {11, 11}, {44, 11}, {44, 28}, {11, 28} });
	policeObejct->SetElement(police, ElementType::Script);

	AudioSource* policeAudioSource = new AudioSource(m_gameEngine);
	policeObejct->SetElement(policeAudioSource, ElementType::AudioSource);
	police->SetAudioSource(policeAudioSource);
	policeAudioSource->AddSound("sfx_siren");
	policeAudioSource->AddSound("sfx_arrest");
	

	//경광등 모션 추가
	Graphics::Motion* policeMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("Police", m_gameEngine->GetImage("Police"), policeMotion);
	police->SetPolice(policeMotion);

	Graphics::Motion* redPoliceMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("RedPolice", m_gameEngine->GetImage("redPolice"), redPoliceMotion);

	police->SetRedPolice(redPoliceMotion);

	Graphics::Motion* bluePoliceMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("BluePolice", m_gameEngine->GetImage("bluePolice"), bluePoliceMotion);
	police->SetBluePolice(bluePoliceMotion);

	// 충돌체
	Collider* policeCollider = m_gameEngine->GetCollisionManager()->CreateIsometricCollider(80.f, 90.f, 72.f);
	m_gameEngine->GetCollisionManager()->AddCollider(policeObejct, policeCollider);
	policeObejct->SetElement(policeCollider, ElementType::Collider);
#pragma endregion Police

#pragma region PathFind
	GameObject* PathFindObject = m_gameEngine->CreateObject("TestNpcManager", thisScene);
	Transform* PathFindTransform = new Transform();
	PathFindTransform->SetUseCamera(true);
	PathFindTransform->SetLayer(10);
	PathFindObject->SetElement(PathFindTransform, ElementType::Transform); \

	PathFindRenderer* pathManager =new PathFindRenderer(playerTransform, policeTrasnform);
	PathFindObject->SetElement(pathManager, ElementType::Script);

	police->SetPathManager(PathFindObject);
#pragma endregion PathFind

#pragma region NpcManager
	GameObject* NpcManagerObject = m_gameEngine->CreateObject("NpcManager", thisScene);
	Transform* NpcManagerTransform = new Transform();
	NpcManagerObject->SetElement(NpcManagerTransform, ElementType::Transform);
	NpcManagerTransform->SetUseCamera(true);
	NpcManager* npcManager = new NpcManager(m_gameEngine, thisScene, m_directY);
	NpcManagerObject->SetElement(npcManager, ElementType::Script);
	npcManager->Init();
	npcManager->ReadSource("Resource/Text/Map/Stage2_npcLayer.txt");
	npcManager->SetPrintLog([=](std::wstring log,bool isNpc) { scrollbar->AddText(log,isNpc); });
	//npcManager->MakeNpc();
	npcManager->MakeObject();
#pragma endregion NpcManager

#pragma region CarNpcManager
	GameObject* CarNpcManagerObject = m_gameEngine->CreateObject("CarNpcManager", thisScene);
	Transform* CarNpcManagerTransform = new Transform();
	CarNpcManagerObject->SetElement(CarNpcManagerTransform, ElementType::Transform);
	CarNpcManagerTransform->SetUseCamera(true);
	NpcManager* CarnpcManager = new NpcManager(m_gameEngine, thisScene, m_directY, NpcType::Car);
	CarNpcManagerObject->SetElement(CarnpcManager, ElementType::Script);
	CarnpcManager->Init();
	CarnpcManager->ReadSource("Resource/Text/Map/Stage2_carLayer.txt");
	CarnpcManager->MakeObject();
#pragma endregion CarNpcManager

#pragma region NpcWaitManager
	///NPC대기열을 관리하는 오브젝트

	GameObject* npcWaitManagerObject = m_gameEngine->CreateObject("NpcWaitManager", thisScene);
	Transform* npcWaitManagerTransform = new Transform;
	npcWaitManagerObject->SetElement(npcWaitManagerTransform, ElementType::Transform);

	NPCWaitManager* npcWaitManager = new NPCWaitManager(npcManager);
	npcWaitManagerObject->SetElement(npcWaitManager, ElementType::Script);
	///면접자를 알아야한다
	heroFaceUI->SetInterviewList(npcWaitManager->GetWaitQueue());
#pragma endregion NpcWaitManager

#pragma region ClearDialogManager
	GameObject* clearDialogManagerObject = m_gameEngine->CreateObject("ClearDialogManager", thisScene);
	Transform* clearDialogManagerTransform = new Transform();
	clearDialogManagerObject->SetElement(clearDialogManagerTransform, ElementType::Transform);
	clearDialogManagerTransform->SetUseCamera(false);
	clearDialogManagerTransform->SetLayer(10000);

	Graphics::Sprite* clearDialogImage = m_directY->ReadSpriteTxtFile("dialog", m_gameEngine->GetImage("dialog"));
	clearDialogImage->m_drawPos = { 960 - dialogImage->m_drawSize.right / 2, 775 };

	Graphics::Sprite* clearDialogGodImage = m_directY->ReadSpriteTxtFile("dialogGod", m_gameEngine->GetImage("dialogGod"));
	clearDialogGodImage->m_drawPos = { 960 - dialogGodImage->m_drawSize.right / 2, 760 };

	DialogManager* clearDialogManager = new DialogManager(0, 0, 1396, 1080, 2100, clearDialogImage, clearDialogGodImage);
	clearDialogManagerObject->SetElement(clearDialogManager, ElementType::Script);
	clearDialogManager->ReadDialogCSV("Resource/Dialog2");
	clearDialogManager->SetTextOffset({ 130 + 960 - dialogGodImage->m_drawSize.right / 2, 860 });
	clearDialogManager->SetStartGame([=]()
		{
			m_directY->DeletePaticleEffect();
			m_gameEngine->SetNextScene("StageScene");
			ClearManager::GetInstance().SetClear(false);
			ClearManager::GetInstance().SetDead(false);

			ClearManager::GetInstance().SetClearStage(2);
		});

	AudioSource* clearDialogManagerAudioSource = new AudioSource(m_gameEngine);
	clearDialogManagerObject->SetElement(clearDialogManagerAudioSource, ElementType::AudioSource);
	clearDialogManager->SetAudioSource(clearDialogManagerAudioSource);
	clearDialogManagerAudioSource->AddSound("sfx_dialog_1");

	//BGI 로 변경
	Graphics::Sprite* cutScene3 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_3"));
	clearDialogManager->AddSprite("CutScene_3", cutScene3);
	Graphics::Sprite* cutScene4 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_4"));
	clearDialogManager->AddSprite("CutScene_4", cutScene4);
	Graphics::Sprite* cutScene5 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_5"));
	clearDialogManager->AddSprite("CutScene_5", cutScene5);
	Graphics::Sprite* cutScene6 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_6"));
	clearDialogManager->AddSprite("CutScene_6", cutScene6);

	clearDialogManagerObject->SetActive(false);
	DebugManager::GetInstance().SetObject(clearDialogManagerObject);
#pragma endregion ClearDialogManager

#pragma region MessageManger
	GameObject* messageManagerObject = m_gameEngine->CreateObject("messageManager", thisScene);
	Transform* messageManagerTransform = new Transform();
	messageManagerObject->SetElement(messageManagerTransform, ElementType::Transform);
	messageManagerTransform->SetUseCamera(true);
	messageManagerTransform->SetLayer(500);

	MessageManager* messageManager = new MessageManager(npcWaitManager, clearDialogManagerObject);
	messageManagerObject->SetElement(messageManager, ElementType::Script);
	messageManager->ReadMessageCSV("Resource/NPC_message_list");
	messageManager->ReadDialogCSV("Resource/NPC_message");
	messageManager->SetQueue(npcWaitManager->GetWaitQueue());
	messageManager->SetClearCount(3);
#pragma endregion MessageManger

#pragma region Target
	GameObject* TargerBarObject = m_gameEngine->CreateObject("TargetBar", thisScene);
	Transform* TargerBarObjectTransform = new Transform();
	TargerBarObject->SetElement(TargerBarObjectTransform, ElementType::Transform);
	TargerBarObjectTransform->SetLocalPosition({ 10,10 });
	TargerBarObjectTransform->SetLayer(1000);

	TargetBar* targetBar = new TargetBar(messageManager, npcManager, npcWaitManager);
	TargerBarObject->SetElement(targetBar, ElementType::Script);

	Graphics::Motion* answerImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("AnswerList", m_gameEngine->GetImage("AnswerList"), answerImage);
	targetBar->SetImage(answerImage);

	Graphics::Motion* toneDownImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("AnswerList", m_gameEngine->GetImage("targettonedown"), toneDownImage);
	targetBar->SetTonedown(toneDownImage);

	Graphics::Motion* targetBackGround = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("targetbar_4", m_gameEngine->GetImage("targetbar_4"), targetBackGround);
	targetBar->SetBackGround(targetBackGround);

#pragma endregion Target
}

void GameProcess::InitGameScene3()
{
	Scene* thisScene = m_gameEngine->FindScene("GameScene3");
	m_gameEngine->LoadResource("GameScene", m_directY);

	m_gameEngine->SetTimeScale(0);

	/// 테스트용 bgm재생	
	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("bgm_ingame_03_loop"), 1.f, PlayType::BGM);

#pragma region DialogManager
	GameObject* dialogManagerObject = m_gameEngine->CreateObject("DialogManager", thisScene);
	Transform* dialogManagerTransform = new Transform();
	dialogManagerObject->SetElement(dialogManagerTransform, ElementType::Transform);
	dialogManagerTransform->SetUseCamera(false);
	dialogManagerTransform->SetLayer(10000);

	Graphics::Sprite* dialogImage = m_directY->ReadSpriteTxtFile("dialog", m_gameEngine->GetImage("dialog"));
	dialogImage->m_drawPos = { 37, 775 };

	Graphics::Sprite* dialogGodImage = m_directY->ReadSpriteTxtFile("dialogGod", m_gameEngine->GetImage("dialogGod"));
	dialogGodImage->m_drawPos = { 30, 760 };

	DialogManager* dialogManager = new DialogManager(0, 0, 1396, 1080, 3000, dialogImage, dialogGodImage);
	dialogManagerObject->SetElement(dialogManager, ElementType::Script);
	dialogManager->ReadDialogCSV("Resource/Dialog2");
	dialogManager->SetTextOffset({ 130, 860 });

	Graphics::Sprite* goddessImageSprite = m_directY->ReadSpriteTxtFile("goddessStanding", m_gameEngine->GetImage("goddess"));
	Graphics::Sprite* goddessDarkImageSprite = m_directY->ReadSpriteTxtFile("goddessStanding", m_gameEngine->GetImage("goddess_Dark"));
	Graphics::Sprite* playerImageSprite = m_directY->ReadSpriteTxtFile("playerStanding", m_gameEngine->GetImage("Player"));
	Graphics::Sprite* playerDarkImageSprite = m_directY->ReadSpriteTxtFile("playerStanding", m_gameEngine->GetImage("Player_Dark"));

	dialogManager->AddSprite("goddess", goddessImageSprite);
	dialogManager->AddSprite("goddess_Dark", goddessDarkImageSprite);
	dialogManager->AddSprite("Player", playerImageSprite);
	dialogManager->AddSprite("Player_Dark", playerDarkImageSprite);

	AudioSource* dialogManagerAudioSource = new AudioSource(m_gameEngine);
	dialogManagerObject->SetElement(dialogManagerAudioSource, ElementType::AudioSource);
	dialogManager->SetAudioSource(dialogManagerAudioSource);
	dialogManagerAudioSource->AddSound("sfx_dialog_1");

#pragma endregion DialogManager

#pragma region UI

#pragma region MainUI
	/// MainUIWindow
	GameObject* UIObject = m_gameEngine->CreateObject("UI", thisScene);
	Transform* uiTransform = new Transform();
	UIObject->SetElement(uiTransform, ElementType::Transform);
	uiTransform->SetLocalPosition({ 1398,40 });
	uiTransform->SetLayer(300);

	InGameUI* mainUI = new InGameUI();
	UIObject->SetElement(mainUI, ElementType::Script);
	Graphics::Sprite* UIImage = m_directY->ReadSpriteTxtFile("ingame_mainsolid", m_gameEngine->GetImage("ingame_mainsolid"));
	mainUI->m_sprite = UIImage;
#pragma endregion MainUI

#pragma region HeroUI
	///면접자 초상화UI
	GameObject* heroPotraitObject = m_gameEngine->CreateObject("heroPotrait", thisScene);
	Transform* heroPotraitTransform = new Transform();
	heroPotraitObject->SetElement(heroPotraitTransform, ElementType::Transform);
	heroPotraitTransform->SetParent(uiTransform);
	heroPotraitTransform->Translate({ 25,446 });
	heroPotraitTransform->SetLayer(312);

	InterviewUI* heroPotraitUI = new InterviewUI();
	heroPotraitObject->SetElement(heroPotraitUI, ElementType::Script);
	Graphics::Motion* interviewImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("heroPotrait", m_gameEngine->GetImage("heroPotrait"), interviewImage);
	heroPotraitUI->SetImage(interviewImage);

	///면접자 얼굴 이미지
	GameObject* heroFaceObject = m_gameEngine->CreateObject("heroPotrait", thisScene);
	Transform* heroFaceTransform = new Transform();
	heroFaceObject->SetElement(heroFaceTransform, ElementType::Transform);
	heroFaceTransform->SetParent(uiTransform);
	heroFaceTransform->Translate({ 35,446 });
	heroFaceTransform->SetLayer(313);

	PotraitUI* heroFaceUI = new PotraitUI();
	heroFaceObject->SetElement(heroFaceUI, ElementType::Script);
	Graphics::Motion* potraitImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("potraitGroup", m_gameEngine->GetImage("potraitGroup"), potraitImage);
	heroFaceUI->SetPotraitGroup(potraitImage);

	///면접자 초상화 프레임
	GameObject* heroFrameObject = m_gameEngine->CreateObject("heroframe", thisScene);
	Transform* heroFrameTransform = new Transform();
	heroFrameObject->SetElement(heroFrameTransform, ElementType::Transform);
	heroFrameTransform->SetParent(uiTransform);
	heroFrameTransform->SetLocalPosition({ 25,446 });
	heroFrameTransform->SetLayer(314);

	InGameUI* heroFrameUI = new InGameUI();
	heroFrameObject->SetElement(heroFrameUI, ElementType::Script);
	Graphics::Sprite* heroFrameImage = m_directY->ReadSpriteTxtFile("heroframe", m_gameEngine->GetImage("heroframe"));
	heroFrameUI->m_sprite = heroFrameImage;
#pragma endregion HeroUI

#pragma region GoddessUI
	///여신 초상화
	GameObject* goddessPotraitObject = m_gameEngine->CreateObject("goddessPotrait", thisScene);
	Transform* goddessPotraitTransform = new Transform();
	goddessPotraitObject->SetElement(goddessPotraitTransform, ElementType::Transform);
	goddessPotraitTransform->SetParent(uiTransform);
	goddessPotraitTransform->Translate({ 305,446 });
	goddessPotraitTransform->SetLayer(312);

	InterviewUI* goddesspotraitUI = new InterviewUI();
	goddessPotraitObject->SetElement(goddesspotraitUI, ElementType::Script);
	Graphics::Motion* goddessImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("goddessPotrait", m_gameEngine->GetImage("goddessPotrait"), goddessImage);
	goddesspotraitUI->SetImage(goddessImage);

	///여신 얼굴 이미지
	GameObject* goddessFaceObject = m_gameEngine->CreateObject("goddessPotrait", thisScene);
	Transform* goddessFaceTransform = new Transform();
	goddessFaceObject->SetElement(goddessFaceTransform, ElementType::Transform);
	goddessFaceTransform->SetParent(uiTransform);
	goddessFaceTransform->Translate({ 315,453 });
	goddessFaceTransform->SetLayer(313);

	InterviewUI* goddessFaceUI = new InterviewUI();
	goddessFaceObject->SetElement(goddessFaceUI, ElementType::Script);
	Graphics::Motion* goddessFaceImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("portrait", m_gameEngine->GetImage("portrait_godness"), goddessFaceImage);
	goddessFaceUI->SetImage(goddessFaceImage);

	///여신 초상화 프레임
	GameObject* goddessBarFrameObject = m_gameEngine->CreateObject("goddessbarframe", thisScene);
	Transform* goddessBarFrameTransform = new Transform();
	goddessBarFrameObject->SetElement(goddessBarFrameTransform, ElementType::Transform);
	goddessBarFrameTransform->SetParent(uiTransform);
	goddessBarFrameTransform->Translate({ 305,446 });
	goddessBarFrameTransform->SetLayer(314);

	InterviewUI* goddessBarFrameUI = new InterviewUI();
	goddessBarFrameObject->SetElement(goddessBarFrameUI, ElementType::Script);
	Graphics::Motion* goddessBarFrameImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("goddessbarframe", m_gameEngine->GetImage("goddessbarframe"), goddessBarFrameImage);
	goddessBarFrameUI->SetImage(goddessBarFrameImage);
#pragma endregion GoddessUI

#pragma region TimerUI
	///Timer
	GameObject* TimerObject = m_gameEngine->CreateObject("Timer", thisScene);
	Transform* TimerTransform = new Transform();
	TimerObject->SetElement(TimerTransform, ElementType::Transform);
	// 부모트랜스폼 설정
	// 트랜스폼 레이어 설정(일단 보일 수 있도록 높은 숫자 넣음)
	TimerTransform->SetLayer(314);
	Timer* timer = new Timer(m_gameEngine, m_directY);
	TimerObject->SetElement(timer, ElementType::Script);

	// 오디오 소스를 추가함
	AudioSource* timerAudioSource = new AudioSource(m_gameEngine);
	TimerObject->SetElement(timerAudioSource, ElementType::AudioSource);
	timer->SetAudioSource(timerAudioSource);
	timerAudioSource->AddSound("bgm_beep_slow");
	timerAudioSource->AddSound("sfx_timer_warning");

	///타이머 장식
	GameObject* secondBaseObject = m_gameEngine->CreateObject("secondHandBase", thisScene);
	Transform* secondBaseTransform = new Transform();
	secondBaseTransform->SetParent(uiTransform);
	secondBaseTransform->SetLocalPosition({ 205 , 622 });
	secondBaseObject->SetElement(secondBaseTransform, ElementType::Transform);
	secondBaseTransform->SetLayer(315);

	InGameUI* secondBaseUI = new InGameUI();
	secondBaseObject->SetElement(secondBaseUI, ElementType::Script);
	Graphics::Sprite* secondBaseImage = m_directY->ReadSpriteTxtFile("secondhand_base", m_gameEngine->GetImage("secondhand_base"));
	secondBaseUI->m_sprite = secondBaseImage;

	///타이머 프레임
	GameObject* clockbaseObject = m_gameEngine->CreateObject("clockbase", thisScene);
	Transform* clockbaseTransform = new Transform();
	clockbaseTransform->SetParent(uiTransform);
	clockbaseTransform->SetLocalPosition({ 120 , 652 });
	clockbaseObject->SetElement(clockbaseTransform, ElementType::Transform);
	clockbaseTransform->SetLayer(314);

	InGameUI* clockbaseUI = new InGameUI();
	clockbaseObject->SetElement(clockbaseUI, ElementType::Script);
	Graphics::Sprite* clockbaseImage = m_directY->ReadSpriteTxtFile("clockbase", m_gameEngine->GetImage("clockbase"));
	clockbaseUI->m_sprite = clockbaseImage;

	dialogManager->SetStartGame([=]()
		{
			m_gameEngine->SetTimeScale(1);
			timer->StartTimer();
		});
#pragma endregion TimerUI

#pragma region MessageUI
	/// 메세지 창 Scrollbar
	GameObject* dialogueObject = m_gameEngine->CreateObject("Scrollbar", thisScene);
	Transform* dialogueTrasform = new Transform();
	dialogueObject->SetElement(dialogueTrasform, ElementType::Transform);
	dialogueTrasform->SetParent(uiTransform);
	dialogueTrasform->Translate({ 25, 35 });
	dialogueTrasform->SetLayer(310);

	Graphics::Sprite* bar = m_directY->ReadSpriteTxtFile("bar", m_gameEngine->GetImage("bar"));
	Graphics::Sprite* background = m_directY->ReadSpriteTxtFile("message", m_gameEngine->GetImage("message"));
	Graphics::Motion* herochatbubble = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("herochatwindow", m_gameEngine->GetImage("chatwindow"), herochatbubble);

	Graphics::Motion* goddesschatbubble = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("herochatwindow", m_gameEngine->GetImage("chatwindow_goddess"), goddesschatbubble);

	Scrollbar* scrollbar = new Scrollbar(35, 50, 422 - 35, 380 - 50, background, bar, herochatbubble, goddesschatbubble);
	dialogueObject->SetElement(scrollbar, ElementType::Scrollbar);
	scrollbar->SetHeroChatWindow(herochatbubble);
	scrollbar->SetGoddessChatWindow(goddesschatbubble);

	/*///메세지 프레임
	GameObject* messageFrameObject = m_gameEngine->CreateObject("heroframe", thisScene);
	Transform* messageFrameTransform = new Transform();
	messageFrameObject->SetElement(messageFrameTransform, ElementType::Transform);
	messageFrameTransform->SetParent(uiTransform);
	messageFrameTransform->SetLocalPosition({ 25,35 });
	messageFrameTransform->SetLayer(312);

	InGameUI* messageFrameUI = new InGameUI();
	messageFrameObject->SetElement(messageFrameUI, ElementType::Script);
	Graphics::Sprite* messageFrameImage = m_directY->ReadSpriteTxtFile("messageframe", m_gameEngine->GetImage("messageframe"));
	messageFrameUI->m_sprite = messageFrameImage;*/
#pragma endregion MessageUI

#pragma region InterviewRoomUI
	///면접장
	GameObject* InterviewRoomObject = m_gameEngine->CreateObject("interviewPlace", thisScene);
	Transform* InterviewRoomTransform = new Transform();
	InterviewRoomTransform->SetParent(uiTransform);
	InterviewRoomTransform->SetLocalPosition({ 36, 672 });
	InterviewRoomObject->SetElement(InterviewRoomTransform, ElementType::Transform);
	InterviewRoomTransform->SetLayer(310);

	InterviewUI* InterviewRoomUI = new InterviewUI();
	InterviewRoomObject->SetElement(InterviewRoomUI, ElementType::Script);
	Graphics::Motion* interviewPlaceImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_bar", m_gameEngine->GetImage("interview_screen_bar"), interviewPlaceImage);
	InterviewRoomUI->SetImage(interviewPlaceImage);

	/// 면접장 배경
	GameObject* InterviewRoomBGObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewRoomBGTransform = new Transform();
	InterviewRoomBGTransform->SetParent(uiTransform);
	InterviewRoomBGTransform->SetLocalPosition({ 35, 673 });
	InterviewRoomBGObject->SetElement(InterviewRoomBGTransform, ElementType::Transform);
	InterviewRoomBGTransform->SetLayer(311);

	InterviewUI* InterviewRoomBGUI = new InterviewUI();
	InterviewRoomBGObject->SetElement(InterviewRoomBGUI, ElementType::Script);
	Graphics::Motion* interviewRoomImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("InterviewRoomBackground", m_gameEngine->GetImage("InterviewRoomBackground"), interviewRoomImage);
	InterviewRoomBGUI->SetImage(interviewRoomImage);

	///면접장 빛
	GameObject* InterviewLightObject = m_gameEngine->CreateObject("interviewLight", thisScene);
	Transform* InterviewLightTransform = new Transform();
	InterviewLightTransform->SetParent(uiTransform);
	InterviewLightTransform->SetLocalPosition({ 35, 673 });
	InterviewLightObject->SetElement(InterviewLightTransform, ElementType::Transform);
	InterviewLightTransform->SetLayer(312);

	InterviewUI* InterviewLightUI = new InterviewUI();
	InterviewLightObject->SetElement(InterviewLightUI, ElementType::Script);
	Graphics::Motion* interviewLightImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interviewLight", m_gameEngine->GetImage("interviewLight"), interviewLightImage);
	InterviewLightUI->SetImage(interviewLightImage);

	/// 면접장 프레임
	GameObject* InterviewFrameObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewFrameTransform = new Transform();
	InterviewFrameTransform->SetParent(uiTransform);
	InterviewFrameTransform->SetLocalPosition({ 25, 535 });
	InterviewFrameObject->SetElement(InterviewFrameTransform, ElementType::Transform);
	InterviewFrameTransform->SetLayer(312);

	InterviewUI* InterviewFrameUI = new InterviewUI();
	InterviewFrameObject->SetElement(InterviewFrameUI, ElementType::Script);
	Graphics::Motion* interviewFrameImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_frame", m_gameEngine->GetImage("interview_screen_frame"), interviewFrameImage);
	InterviewFrameUI->SetImage(interviewFrameImage);

	///면접장 데코
	GameObject* InterviewDecoObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewDecoTransform = new Transform();
	InterviewDecoTransform->SetParent(uiTransform);
	InterviewDecoTransform->SetLocalPosition({ 20, 652 });
	InterviewDecoObject->SetElement(InterviewDecoTransform, ElementType::Transform);
	InterviewDecoTransform->SetLayer(313);

	InterviewUI* InterviewDecoUI = new InterviewUI();
	InterviewDecoObject->SetElement(InterviewDecoUI, ElementType::Script);
	Graphics::Motion* interviewDecoImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_frame_deco", m_gameEngine->GetImage("interview_screen_frame_deco"), interviewDecoImage);
	InterviewDecoUI->SetImage(interviewDecoImage);
#pragma endregion InterviewRoomUI

#pragma  region MainOptionUI

	GameObject* MainOptionUIObject = m_gameEngine->CreateObject("MainOptionUI", thisScene);
	Transform* MainOptionUITransform = new Transform;
	MainOptionUIObject->SetElement(MainOptionUITransform, ElementType::Transform);
	MainOptionUITransform->SetLayer(500);
	MainOptionUITransform->SetLocalPosition({ 960 - 315,540 - 215 });
	MainOptionUIObject->SetActive(false);

	MainOption* mainOptionUI = new MainOption();
	MainOptionUIObject->SetElement(mainOptionUI, ElementType::Script);
	Graphics::Sprite* mainOptionImage = m_directY->ReadSpriteTxtFile("menubar_all", m_gameEngine->GetImage("menubar_all"));
	mainOptionUI->SetImage(mainOptionImage);

	/// 타이틀로 가기 옵션 버튼
	GameObject* mainButtonObject = m_gameEngine->CreateObject("MainTitleButton", thisScene);
	mainButtonObject->SetActive(false);

	Transform* mainButtonTransform = new Transform();
	mainButtonObject->SetElement(mainButtonTransform, ElementType::Transform);
	mainButtonTransform->SetParent(MainOptionUITransform);
	mainButtonTransform->SetLocalPosition({ 185, 125 });

	Vector2 mainButtonWorldPosition = mainButtonTransform->GetWorldPosition();
	Button* mainButton = new Button(0 + (int)mainButtonWorldPosition.x, 0 + (int)mainButtonWorldPosition.y, 266 + (int)mainButtonWorldPosition.x, 71 + (int)mainButtonWorldPosition.y);
	mainButtonObject->SetElement(mainButton, ElementType::Script);
	mainButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("TitleScene");
			}
		});

	// 메인버튼 오디오소스
	AudioSource* mainButtonAudioSource = new AudioSource(m_gameEngine);
	mainButtonObject->SetElement(mainButtonAudioSource, ElementType::AudioSource);
	mainButton->SetAudioSource(mainButtonAudioSource);
	mainButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	/// 다시하기 옵션 버튼
	GameObject* replayButtonObject = m_gameEngine->CreateObject("ReplayButton", thisScene);
	replayButtonObject->SetActive(false);

	Transform* replayButtonTransform = new Transform();
	replayButtonObject->SetElement(replayButtonTransform, ElementType::Transform);
	replayButtonTransform->SetParent(MainOptionUITransform);
	replayButtonTransform->SetLocalPosition({ 185, 215 });

	Vector2 replaybuttonWorldPosition = replayButtonTransform->GetWorldPosition();
	Button* replayButton = new Button(0 + (int)replaybuttonWorldPosition.x, 0 + (int)replaybuttonWorldPosition.y, 266 + (int)replaybuttonWorldPosition.x, 71 + (int)replaybuttonWorldPosition.y);
	replayButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("GameScene3");
			}
		});
	replayButtonObject->SetElement(replayButton, ElementType::Script);

	// 다시하기 버튼 오디오소스
	AudioSource* replayButtonAudioSource = new AudioSource(m_gameEngine);
	replayButtonObject->SetElement(replayButtonAudioSource, ElementType::AudioSource);
	replayButton->SetAudioSource(replayButtonAudioSource);
	replayButtonAudioSource->AddSoundVector("sfx_OnButton");
	replayButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	///옵션 닫기
	GameObject* exitButtonObject = m_gameEngine->CreateObject("exitbutton", thisScene);

	Transform* exitbuttonTransform = new Transform();
	exitButtonObject->SetElement(exitbuttonTransform, ElementType::Transform);
	exitbuttonTransform->SetLayer(501);
	exitButtonObject->SetActive(false);

	Button* exitButton = new Button(1240, 325, 1276, 360);
	Graphics::Sprite* exiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	exitButton->m_sprite = exiteButtonImage;
	exitButtonObject->SetElement(exitButton, ElementType::Button);

	AudioSource* exitButtonAudioSource = new AudioSource(m_gameEngine);
	exitButtonObject->SetElement(exitButtonAudioSource, ElementType::AudioSource);
	exitButton->SetAudioSource(exitButtonAudioSource);
	exitButtonAudioSource->AddSoundVector("sfx_OnButton");
	exitButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	/// 우상단 옵션 버튼
	GameObject* MainButtonObject = m_gameEngine->CreateObject("mainbutton", thisScene);
	Transform* MainButtonTransform = new Transform();
	MainButtonObject->SetElement(MainButtonTransform, ElementType::Transform);
	MainButtonTransform->SetLayer(500);

	Button* MainOptionButton = new Button(1865, 15, 1905, 55);
	Graphics::Sprite* MainButtonImage = m_directY->ReadSpriteTxtFile("button", m_gameEngine->GetImage("menubutton"));
	MainOptionButton->m_sprite = MainButtonImage;
	MainButtonObject->SetElement(MainOptionButton, ElementType::Button);

	AudioSource* mainOptionButtonAudioSource = new AudioSource(m_gameEngine);
	MainButtonObject->SetElement(mainOptionButtonAudioSource, ElementType::AudioSource);
	MainOptionButton->SetAudioSource(mainOptionButtonAudioSource);
	mainOptionButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainOptionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion MainOptionUI	

#pragma region SoundOptionUI
	/// 옵션UI생성
	/// OptionUIObject 생성
	GameObject* SoundOptionUIObject = m_gameEngine->CreateObject("optionUI", thisScene);
	Transform* soundOptionUITransform = new Transform;
	SoundOptionUIObject->SetElement(soundOptionUITransform, ElementType::Transform);
	soundOptionUITransform->SetLayer(510);
	soundOptionUITransform->SetLocalPosition({ (1920 / 2) - 315.5f ,(1080 / 2) - 215.5f });

	// 볼륨슬라이더 오브젝트 생성
	GameObject* MasterVolumeSliderObject = m_gameEngine->CreateObject("MsterVolumeSlider", thisScene);
	GameObject* BGMVolumeSliderObject = m_gameEngine->CreateObject("BGMVolumeSlider", thisScene);
	GameObject* SFXVolumeSliderObject = m_gameEngine->CreateObject("SFXVolumeSlider", thisScene);

	// 슬라이더 트랜스폼
	Transform* masterVolumeSliderTransform = new Transform;
	Transform* BGMVolumeSliderTransform = new Transform;
	Transform* SFXVolumeSliderTransform = new Transform;

	MasterVolumeSliderObject->SetElement(masterVolumeSliderTransform, ElementType::Transform);
	BGMVolumeSliderObject->SetElement(BGMVolumeSliderTransform, ElementType::Transform);
	SFXVolumeSliderObject->SetElement(SFXVolumeSliderTransform, ElementType::Transform);

	masterVolumeSliderTransform->SetParent(soundOptionUITransform);
	BGMVolumeSliderTransform->SetParent(soundOptionUITransform);
	SFXVolumeSliderTransform->SetParent(soundOptionUITransform);

	masterVolumeSliderTransform->SetLayer(511);
	BGMVolumeSliderTransform->SetLayer(511);
	SFXVolumeSliderTransform->SetLayer(511);

	// 위치를 미리 찍는다.
	masterVolumeSliderTransform->SetLocalPosition({ 160, 100 });
	BGMVolumeSliderTransform->SetLocalPosition({ 160, 200 });
	SFXVolumeSliderTransform->SetLocalPosition({ 160, 300 });

	// 볼륨슬라이더 리소스 생성
	// (슬라이더 리소스들의 해제가 이루어지지 않고 있음)
	// ui
	Graphics::Sprite* soundOptionUIBackground = m_directY->ReadSpriteTxtFile("sounduibar", m_gameEngine->GetImage("sounduibar"));
	// slider
	Graphics::Sprite* sliderBackground_Master = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_BGM = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_SFX = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderFillArea_Master = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_Master = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_BGM = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_BGM = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_SFX = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_SFX = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));

	Graphics::Sprite* BGImage = m_directY->ReadSpriteTxtFile("bgsound", m_gameEngine->GetImage("bgsound")); //배경음 글자 이미지
	Graphics::Sprite* EffectImage = m_directY->ReadSpriteTxtFile("effectsound", m_gameEngine->GetImage("effectsound")); //효과음 글자 이미지
	Graphics::Sprite* MasterImage = m_directY->ReadSpriteTxtFile("mastersound", m_gameEngine->GetImage("mastersound")); //마스터 글자 이미지

	// 슬라이더 AA
	Slider* masterVolumeSlider = new Slider((int)sliderBackground_Master->m_drawSize.left, (int)sliderBackground_Master->m_drawSize.right,
		(int)sliderBackground_Master->m_drawSize.top, (int)sliderBackground_Master->m_drawSize.bottom,
		(int)(sliderBackground_Master->m_drawSize.left + sliderBackground_Master->m_drawSize.right) / 2,
		(int)(sliderBackground_Master->m_drawSize.top, sliderBackground_Master->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(MASTER));
	Slider* SFXVolumeSlider = new Slider((int)sliderBackground_SFX->m_drawSize.left, (int)sliderBackground_SFX->m_drawSize.right,
		(int)sliderBackground_SFX->m_drawSize.top, (int)sliderBackground_SFX->m_drawSize.bottom,
		(int)(sliderBackground_SFX->m_drawSize.left + sliderBackground_SFX->m_drawSize.right) / 2,
		(int)(sliderBackground_SFX->m_drawSize.top, sliderBackground_SFX->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(SFX));
	Slider* BGMVolumeSlider = new Slider((int)sliderBackground_BGM->m_drawSize.left, (int)sliderBackground_BGM->m_drawSize.right,
		(int)sliderBackground_BGM->m_drawSize.top, (int)sliderBackground_BGM->m_drawSize.bottom,
		(int)(sliderBackground_BGM->m_drawSize.left + sliderBackground_BGM->m_drawSize.right) / 2,
		(int)(sliderBackground_BGM->m_drawSize.top, sliderBackground_BGM->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(BGM));

	// 슬라이더 이미지 저장
	masterVolumeSlider->SetBackground(sliderBackground_Master);
	masterVolumeSlider->SetFillArea(sliderFillArea_Master);
	masterVolumeSlider->SetHandle(sliderBar_Master);

	BGMVolumeSlider->SetBackground(sliderBackground_BGM);
	BGMVolumeSlider->SetFillArea(sliderFillArea_BGM);
	BGMVolumeSlider->SetHandle(sliderBar_BGM);

	SFXVolumeSlider->SetBackground(sliderBackground_SFX);
	SFXVolumeSlider->SetFillArea(sliderFillArea_SFX);
	SFXVolumeSlider->SetHandle(sliderBar_SFX);

	MasterVolumeSliderObject->SetElement(masterVolumeSlider, ElementType::Slider);
	BGMVolumeSliderObject->SetElement(BGMVolumeSlider, ElementType::Slider);
	SFXVolumeSliderObject->SetElement(SFXVolumeSlider, ElementType::Slider);

	// 슬라이더 오브젝트만 가져서 수치 반영만 한다.
	SoundOptionUI* soundOptionUI = new SoundOptionUI(masterVolumeSlider, BGMVolumeSlider, SFXVolumeSlider);
	SoundOptionUIObject->SetElement(soundOptionUI, ElementType::Script);
	soundOptionUI->SetUISprite(soundOptionUIBackground);
	soundOptionUI->SetMasterTiltle(MasterImage);
	soundOptionUI->SetBgmTiltle(BGImage);
	soundOptionUI->SetSfxTiltle(EffectImage);

	soundOptionUI->Init();

	/// 사운드 옵션 끄기 버튼
	GameObject* soundExitButtonObject = m_gameEngine->CreateObject("soundExitbutton", thisScene);
	Transform* soundExitbuttonTransform = new Transform();
	soundExitButtonObject->SetElement(soundExitbuttonTransform, ElementType::Transform);
	soundExitbuttonTransform->SetLocalPosition({ 597, 0 });
	soundExitbuttonTransform->SetLayer(511);
	soundExitButtonObject->SetActive(false);
	soundExitButtonObject->SetIsDraw(false);

	Button* soundExitButton = new Button(1240, 325, 1275, 360);
	Graphics::Sprite* soundExiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	soundExitButton->m_sprite = soundExiteButtonImage;
	soundExitButton->SetOnClick([=]()
		{
			if (SoundOptionUIObject->GetActive())
			{
				MainOptionUIObject->SetActive(true);

				soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
				exitButtonObject->SetActive(!exitButtonObject->GetActive());
				SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetActive());
				MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
				SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
				BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
			}
		});
	soundExitButtonObject->SetElement(soundExitButton, ElementType::Button);

	AudioSource* soundExitButtonAudiosource = new AudioSource(m_gameEngine);
	soundExitButtonObject->SetElement(soundExitButtonAudiosource, ElementType::AudioSource);
	soundExitButton->SetAudioSource(soundExitButtonAudiosource);
	soundExitButtonAudiosource->AddSoundVector("sfx_OnButton");
	soundExitButtonAudiosource->AddSoundVector("sfx_StageSelect_click");

	/// 음량 옵션 버튼
	GameObject* ButtonObject = m_gameEngine->CreateObject("soundbutton", thisScene);
	Transform* buttonTransform = new Transform();
	ButtonObject->SetActive(false);
	ButtonObject->SetElement(buttonTransform, ElementType::Transform);
	buttonTransform->SetParent(MainOptionUITransform);
	buttonTransform->SetLocalPosition({ 185, 310 });
	buttonTransform->SetLayer(501);
	Button* soundOptionButton = new Button(830, 635, 1095, 705);
	//Graphics::Sprite* buttonImage = m_directY->ReadSpriteTxtFile("soundbutton", m_gameEngine->GetImage("soundbutton"));
	//soundOptionButton->m_sprite = buttonImage;
	soundOptionButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				MainOptionUIObject->SetActive(false);

				exitButtonObject->SetActive(!exitButtonObject->GetActive());
				soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
				SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetIsDraw());
				MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
				SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
				BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
			}
		});
	ButtonObject->SetElement(soundOptionButton, ElementType::Button);

	AudioSource* soundOptionButtonAudioSource = new AudioSource(m_gameEngine);
	ButtonObject->SetElement(soundOptionButtonAudioSource, ElementType::AudioSource);
	soundOptionButton->SetAudioSource(soundOptionButtonAudioSource);
	soundOptionButtonAudioSource->AddSoundVector("sfx_OnButton");
	soundOptionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion SoundOptionUI

#pragma endregion OnClick
	MainOptionButton->SetOnClick([=]()
		{
			MainOptionUIObject->SetActive(true);
			exitButtonObject->SetActive(true);

			mainButtonObject->SetActive(true);
			replayButtonObject->SetActive(true);
			exitButtonObject->SetActive(true);
			ButtonObject->SetActive(true);
		});
	exitButton->SetOnClick([=]()
		{
			MainOptionUIObject->SetActive(false);
			exitButtonObject->SetActive(false);

			mainButtonObject->SetActive(false);
			replayButtonObject->SetActive(false);
			exitButtonObject->SetActive(false);
			ButtonObject->SetActive(false);
		});
#pragma endregion OnClick
#pragma endregion UI

#pragma region Player
	GameObject* PlayerObject = m_gameEngine->CreateObject("Player", thisScene);

	Transform* playerTransform = new Transform();
	PlayerObject->SetElement(playerTransform, ElementType::Transform);
	playerTransform->SetUseCamera(true);
	playerTransform->SetLayer(5);
	playerTransform->SetLocalPosition({ 72.f, 900.f });

	Player* player = new Player();
	PlayerObject->SetElement(player, ElementType::Script);
	player->SetMyTransform(playerTransform);

	Graphics::Motion* truckMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("isekaiTruck", m_gameEngine->GetImage("Truck"), truckMotion);
	player->SetMotion(truckMotion);

	player->SetMyPaticle(m_directY->ReadSpriteTxtFile("starB", m_gameEngine->GetImage("starB")));
	player->SetMyPaticle2(m_directY->ReadSpriteTxtFile("starM", m_gameEngine->GetImage("starM")));

	for (int i = 0; i < 32; i++)
	{
		truckMotion->GetSprite(i)->m_drawPos.x -= truckMotion->GetSprite(i)->m_pivot.x;
		truckMotion->GetSprite(i)->m_drawPos.y -= truckMotion->GetSprite(i)->m_pivot.y;
	}

	Collider* playerCollider = m_gameEngine->GetCollisionManager()->CreateVehicleCollider(0.f, 0.f, 100, 200.f);
	m_gameEngine->GetCollisionManager()->SetPlayerCollider(playerCollider);
	m_gameEngine->GetCollisionManager()->SetPlayerObject(PlayerObject);
	PlayerObject->SetElement(playerCollider, ElementType::Collider);

	AudioSource* playerAudioScource = new AudioSource(m_gameEngine);
	PlayerObject->SetElement(playerAudioScource, ElementType::AudioSource);
	player->SetAudioSource(playerAudioScource);
	playerAudioScource->AddSound("sfx_hit_car_1");
	playerAudioScource->AddSound("sfx_hit_car_1");
	playerAudioScource->AddSound("sfx_hit_npc_1");
	playerAudioScource->AddSound("sfx_hit_npc_1");
	playerAudioScource->AddSound("sfx_hit_wall_1");
	playerAudioScource->AddSound("sfx_truck_start");
	playerAudioScource->AddSound("sfx_engineSoundMId_loop");
	playerAudioScource->AddSound("sfx_engineSoundBase_loop");
	playerAudioScource->AddSound("sfx_trucBack_loop");

#pragma endregion Player

#pragma region GroundRenderManager
	GameObject* GroundRenderManagerObject = m_gameEngine->CreateObject("GroundRenderManager", thisScene);
	Transform* GroundRenderManagerTransform = new Transform();
	GroundRenderManagerObject->SetElement(GroundRenderManagerTransform, ElementType::Transform);
	GroundRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	GroundRenderManagerTransform->SetLayer(0);
	TileRenderManager* groundRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY);
	GroundRenderManagerObject->SetElement(groundRenderManager, ElementType::Script);
	groundRenderManager->Init();
	groundRenderManager->ReadSource("Resource/Text/Map/Stage3_tile.txt");

#pragma endregion GroundRenderManager

#pragma region ZoneRenderManager
	GameObject* ZoneRenderManagerObject = m_gameEngine->CreateObject("ZoneRenderManager", thisScene);
	Transform* ZoneRenderManagerTransform = new Transform();
	ZoneRenderManagerObject->SetElement(ZoneRenderManagerTransform, ElementType::Transform);
	ZoneRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	ZoneRenderManagerTransform->SetLayer(6);
	TileRenderManager* zoneRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY);
	ZoneRenderManagerObject->SetElement(zoneRenderManager, ElementType::Script);
	zoneRenderManager->Init();
	zoneRenderManager->ReadSource("Resource/Text/Map/Stage3_zone.txt");
#pragma endregion ZoneRenderManager

#pragma region TileRenderManager
	GameObject* TileRenderManagerObject = m_gameEngine->CreateObject("TileRenderManager", thisScene);
	Transform* TileRenderManagerTransform = new Transform();
	TileRenderManagerObject->SetElement(TileRenderManagerTransform, ElementType::Transform);
	TileRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	TileRenderManagerTransform->SetLayer(6);
	TileRenderManager* tileRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY,TileRenderState::Object);
	TileRenderManagerObject->SetElement(tileRenderManager, ElementType::Script);
	tileRenderManager->Init();
	tileRenderManager->ReadSource("Resource/Text/Map/Stage3_object.txt");
#pragma endregion TileRenderManager

#pragma region TileManager
	GameObject* TileManagerObject = m_gameEngine->CreateObject("TileManager", thisScene);
	Transform* TileManagerTransform = new Transform();
	TileManagerObject->SetElement(TileManagerTransform, ElementType::Transform);
	TileManagerTransform->SetUseCamera(true);
	TileManagerTransform->SetLayer(2);
	// 개별 타일의 오브젝트 및 콜라이더를 tileManager Script 안에서 생성함
	TileManager* tileManager = new TileManager(m_gameEngine, m_directY);
	TileManagerObject->SetElement(tileManager, ElementType::Script);
	tileManager->ReadSource2("Resource/Text/Map/Stage3_check.txt");
	tileManager->Init();
#pragma endregion TileManager
#pragma region Cloud
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud1", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ -1900.f, 700.f });
		cloudTransform->SetLayer(300);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud1_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud2", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ 2400.f, 3500.f });
		cloudTransform->SetLayer(300);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud2_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud3", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ -2400.f, 3500.f });
		cloudTransform->SetLayer(300);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud1_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
	{
		GameObject* cloudObject = m_gameEngine->CreateObject("cloud4", thisScene);
		Transform* cloudTransform = new Transform();
		cloudObject->SetElement(cloudTransform, ElementType::Transform);
		cloudTransform->SetUseCamera(true);
		cloudTransform->Translate({ 1900.f, 700.f });
		cloudTransform->SetLayer(300);
		CloudObject* cloudScript = new CloudObject(m_gameEngine, m_directY, CloudObject::State::cloud1_1);
		cloudObject->SetElement(cloudScript, ElementType::Script);
	}
#pragma endregion Cloud
#pragma region Police
	GameObject* policeObejct = m_gameEngine->CreateObject("Police", thisScene);
	Transform* policeTrasnform = new Transform();
	policeObejct->SetElement(policeTrasnform, ElementType::Transform);
	policeTrasnform->SetUseCamera(true);
	policeTrasnform->SetLayer(8);
	policeTrasnform->SetLocalPosition({ -864.f, 3024.f });

	Police* police = new Police(playerTransform, policeTrasnform, policeTrasnform->GetLocalPosition(), 
		"Resource/Text/Map/Stage3_check.txt", { {49, 34}, {49, 24}, {39, 24}, {39, 14}, {14, 14}, {14, 49}, {49, 49}, {49, 45} });
	policeObejct->SetElement(police, ElementType::Script);

	AudioSource* policeAudioSource = new AudioSource(m_gameEngine);
	policeObejct->SetElement(policeAudioSource, ElementType::AudioSource);
	police->SetAudioSource(policeAudioSource);
	policeAudioSource->AddSound("sfx_siren");
	policeAudioSource->AddSound("sfx_arrest");

	//경광등 모션 추가
	Graphics::Motion* policeMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("Police", m_gameEngine->GetImage("Police"), policeMotion);
	police->SetPolice(policeMotion);

	Graphics::Motion* redPoliceMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("RedPolice", m_gameEngine->GetImage("redPolice"), redPoliceMotion);

	police->SetRedPolice(redPoliceMotion);

	Graphics::Motion* bluePoliceMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("BluePolice", m_gameEngine->GetImage("bluePolice"), bluePoliceMotion);
	police->SetBluePolice(bluePoliceMotion);

	// 충돌체
	Collider* policeCollider = m_gameEngine->GetCollisionManager()->CreateIsometricCollider(80.f, 90.f, 72.f);
	m_gameEngine->GetCollisionManager()->AddCollider(policeObejct, policeCollider);
	policeObejct->SetElement(policeCollider, ElementType::Collider);
#pragma endregion Police

#pragma region PathFind
	GameObject* PathFindObject = m_gameEngine->CreateObject("TestNpcManager", thisScene);
	Transform* PathFindTransform = new Transform();
	PathFindTransform->SetUseCamera(true);
	PathFindTransform->SetLayer(10);
	PathFindObject->SetElement(PathFindTransform, ElementType::Transform);

	PathFindRenderer* pathManager = new PathFindRenderer( playerTransform, policeTrasnform);
	PathFindObject->SetElement(pathManager, ElementType::Script);

	police->SetPathManager(PathFindObject);
#pragma endregion PathFind

#pragma region NpcManager
	GameObject* NpcManagerObject = m_gameEngine->CreateObject("NpcManager", thisScene);
	Transform* NpcManagerTransform = new Transform();
	NpcManagerObject->SetElement(NpcManagerTransform, ElementType::Transform);
	NpcManagerTransform->SetUseCamera(true);
	NpcManager* npcManager = new NpcManager(m_gameEngine, thisScene, m_directY);
	NpcManagerObject->SetElement(npcManager, ElementType::Script);
	npcManager->Init();
	npcManager->ReadSource("Resource/Text/Map/Stage3_npcLayer.txt");
	npcManager->SetPrintLog([=](std::wstring log, bool isNpc) { scrollbar->AddText(log,isNpc); });
	//npcManager->MakeNpc();
	npcManager->MakeObject();
#pragma endregion NpcManager

#pragma region CarNpcManager
	GameObject* CarNpcManagerObject = m_gameEngine->CreateObject("CarNpcManager", thisScene);
	Transform* CarNpcManagerTransform = new Transform();
	CarNpcManagerObject->SetElement(CarNpcManagerTransform, ElementType::Transform);
	CarNpcManagerTransform->SetUseCamera(true);
	NpcManager* CarnpcManager = new NpcManager(m_gameEngine, thisScene, m_directY, NpcType::Car);
	CarNpcManagerObject->SetElement(CarnpcManager, ElementType::Script);
	CarnpcManager->Init();
	CarnpcManager->ReadSource("Resource/Text/Map/Stage3_carLayer.txt");
	CarnpcManager->MakeObject();
#pragma endregion CarNpcManager

#pragma region NpcWaitManager
	///NPC대기열을 관리하는 오브젝트

	GameObject* npcWaitManagerObject = m_gameEngine->CreateObject("NpcWaitManager", thisScene);
	Transform* npcWaitManagerTransform = new Transform;
	npcWaitManagerObject->SetElement(npcWaitManagerTransform, ElementType::Transform);

	NPCWaitManager* npcWaitManager = new NPCWaitManager(npcManager);
	npcWaitManagerObject->SetElement(npcWaitManager, ElementType::Script);

	///면접자를 알아야한다
	heroFaceUI->SetInterviewList(npcWaitManager->GetWaitQueue());
#pragma endregion NpcWaitManager

#pragma region ClearDialogManager
	GameObject* clearDialogManagerObject = m_gameEngine->CreateObject("ClearDialogManager", thisScene);
	Transform* clearDialogManagerTransform = new Transform();
	clearDialogManagerObject->SetElement(clearDialogManagerTransform, ElementType::Transform);
	clearDialogManagerTransform->SetUseCamera(false);
	clearDialogManagerTransform->SetLayer(10000);

	Graphics::Sprite* clearDialogImage = m_directY->ReadSpriteTxtFile("dialog", m_gameEngine->GetImage("dialog"));
	clearDialogImage->m_drawPos = { 960 - dialogImage->m_drawSize.right / 2, 775 };

	Graphics::Sprite* clearDialogGodImage = m_directY->ReadSpriteTxtFile("dialogGod", m_gameEngine->GetImage("dialogGod"));
	clearDialogGodImage->m_drawPos = { 960 - dialogGodImage->m_drawSize.right / 2, 760 };

	DialogManager* clearDialogManager = new DialogManager(0, 0, 1396, 1080, 3100, clearDialogImage, clearDialogGodImage);
	clearDialogManagerObject->SetElement(clearDialogManager, ElementType::Script);
	clearDialogManager->ReadDialogCSV("Resource/Dialog2");
	clearDialogManager->SetTextOffset({ 130 + 960 - dialogGodImage->m_drawSize.right / 2, 860 });
	clearDialogManager->SetStartGame([=]()
		{
			m_directY->DeletePaticleEffect();
			m_gameEngine->SetNextScene("GameClearScene");
			ClearManager::GetInstance().SetClear(false);
			ClearManager::GetInstance().SetDead(false);

			ClearManager::GetInstance().SetClearStage(3);
		});

	AudioSource* clearDialogManagerAudioSource = new AudioSource(m_gameEngine);
	clearDialogManagerObject->SetElement(clearDialogManagerAudioSource, ElementType::AudioSource);
	clearDialogManager->SetAudioSource(clearDialogManagerAudioSource);
	clearDialogManagerAudioSource->AddSound("sfx_dialog_1");

	//BGI 로 변경
	Graphics::Sprite* cutScene7 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_7"));
	clearDialogManager->AddSprite("CutScene_7", cutScene7);
	Graphics::Sprite* cutScene8 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_8"));
	clearDialogManager->AddSprite("CutScene_8", cutScene8);
	Graphics::Sprite* cutScene9 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_9"));
	clearDialogManager->AddSprite("CutScene_9", cutScene9);
	Graphics::Sprite* cutScene10 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("CutScene_10"));
	clearDialogManager->AddSprite("CutScene_10", cutScene10);

	clearDialogManagerObject->SetActive(false);
	DebugManager::GetInstance().SetObject(clearDialogManagerObject);
#pragma endregion ClearDialogManager

#pragma region MessageManger
	GameObject* messageManagerObject = m_gameEngine->CreateObject("messageManager", thisScene);
	Transform* messageManagerTransform = new Transform();
	messageManagerObject->SetElement(messageManagerTransform, ElementType::Transform);
	messageManagerTransform->SetUseCamera(true);
	messageManagerTransform->SetLayer(500);

	MessageManager* messageManager = new MessageManager(npcWaitManager, clearDialogManagerObject);
	messageManagerObject->SetElement(messageManager, ElementType::Script);
	messageManager->ReadMessageCSV("Resource/NPC_message_list");
	messageManager->ReadDialogCSV("Resource/NPC_message");
	messageManager->SetQueue(npcWaitManager->GetWaitQueue());
	messageManager->SetClearCount(4);
#pragma endregion MessageManger

#pragma region Target
	GameObject* TargerBarObject = m_gameEngine->CreateObject("TargetBar", thisScene);
	Transform* TargerBarObjectTransform = new Transform();
	TargerBarObject->SetElement(TargerBarObjectTransform, ElementType::Transform);
	TargerBarObjectTransform->SetLocalPosition({ 10,10 });
	TargerBarObjectTransform->SetLayer(1000);

	TargetBar* targetBar = new TargetBar(messageManager, npcManager, npcWaitManager);
	TargerBarObject->SetElement(targetBar, ElementType::Script);

	Graphics::Motion* answerImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("AnswerList", m_gameEngine->GetImage("AnswerList"), answerImage);
	targetBar->SetImage(answerImage);

	Graphics::Motion* toneDownImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("AnswerList", m_gameEngine->GetImage("targettonedown"), toneDownImage);
	targetBar->SetTonedown(toneDownImage);

	Graphics::Motion* targetBackGround = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("targetbar_4", m_gameEngine->GetImage("targetbar_4"), targetBackGround);
	targetBar->SetBackGround(targetBackGround);

#pragma endregion Target

}

void GameProcess::InitTitleScene()
{
	Scene* thisScene = m_gameEngine->FindScene("TitleScene");
	m_gameEngine->LoadResource("TitleScene", m_directY);

	m_gameEngine->SetTimeScale(1);

	/// 테스트용 bgm재생	
	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("bgm_title_loop"), 1.f, PlayType::BGM);

#pragma region UI

#pragma region Title
	// 타이틀 (이미지)
	GameObject* titleObject = m_gameEngine->CreateObject("Title", thisScene);

	// Translate 해야함
	Transform* titleTransform = new Transform();
	titleObject->SetElement(titleTransform, ElementType::Transform);

	Graphics::Sprite* titleImage = m_directY->ReadSpriteTxtFile("Title", m_gameEngine->GetImage("title"));
	BackgroundImage* title = new BackgroundImage(titleImage);
	titleObject->SetElement(title, ElementType::Script);
	// Sprite 출력 해야함
#pragma endregion
#pragma region StartButton
	// 게임시작 (버튼)
	GameObject* startButtonObject = m_gameEngine->CreateObject("StartButton", thisScene);

	Transform* startButtonTransform = new Transform();
	startButtonObject->SetElement(startButtonTransform, ElementType::Transform);

	// 생성자 값 다시 넣어야 함
	Button* startButton = new Button(770, 570, 770 + 381, 570 + 111);
	startButton->SetOnClick([=]()
		{
			m_gameEngine->SetNextScene("OpeningScene");
		});
	startButtonObject->SetElement(startButton, ElementType::Button);

	AudioSource* startButtonAudiosource = new AudioSource(m_gameEngine);
	startButtonObject->SetElement(startButtonAudiosource, ElementType::AudioSource);
	startButton->SetAudioSource(startButtonAudiosource);
	startButtonAudiosource->AddSoundVector("sfx_OnButton");
	startButtonAudiosource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion
#pragma region DataReset
	// 초기화 팝업
	GameObject* dataResetPopupObject = m_gameEngine->CreateObject("DataResetPopup", thisScene);

	Transform* dataResetPopupTransform = new Transform();
	dataResetPopupObject->SetElement(dataResetPopupTransform, ElementType::Transform);

	Graphics::Sprite* dataResetPopupImage = m_directY->ReadSpriteTxtFile("dataResetPopup", m_gameEngine->GetImage("dataresetPopup"));
	BackgroundImage* dataPopup = new BackgroundImage(dataResetPopupImage);
	dataResetPopupObject->SetElement(dataPopup, ElementType::Script);

	dataResetPopupTransform->SetLayer(100000);
	dataResetPopupTransform->Translate({ 960 - dataResetPopupImage->m_drawSize.right / 2, 540 - dataResetPopupImage->m_drawSize.bottom / 2 });

	Button* dataResetExitButton = new Button(1240, 327, 1240 + 36, 327 + 36);
	dataResetExitButton->SetOnClick([=]()
		{
			dataResetPopupObject->SetActive(false);
		});
	dataResetPopupObject->SetElement(dataResetExitButton, ElementType::Button);

	AudioSource* dataResetButtonAudiosource = new AudioSource(m_gameEngine);
	dataResetPopupObject->SetElement(dataResetButtonAudiosource, ElementType::AudioSource);
	dataResetExitButton->SetAudioSource(dataResetButtonAudiosource);
	dataResetButtonAudiosource->AddSoundVector("sfx_OnButton");
	dataResetButtonAudiosource->AddSoundVector("sfx_StageSelect_click");

	dataResetPopupObject->SetActive(false);

	// 데이터 초기화 (버튼)
	GameObject* dataResetButtonObject = m_gameEngine->CreateObject("DataResetButton", thisScene);

	Transform* dataResetButtonTransform = new Transform();
	dataResetButtonObject->SetElement(dataResetButtonTransform, ElementType::Transform);

	Button* dataResetButton = new Button(770, 715, 770 + 381, 715 + 111);
	dataResetButtonObject->SetElement(dataResetButton, ElementType::Button);

	/*AudioSource* */dataResetButtonAudiosource = new AudioSource(m_gameEngine);
	dataResetButtonObject->SetElement(dataResetButtonAudiosource, ElementType::AudioSource);
	dataResetButton->SetAudioSource(dataResetButtonAudiosource);
	dataResetButtonAudiosource->AddSoundVector("sfx_OnButton");
	dataResetButtonAudiosource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion
#pragma region ExitButton
	// 게임 종료 (버튼)
	GameObject* exitButtonObject = m_gameEngine->CreateObject("ExitButton", thisScene);

	Transform* exitButtonTransform = new Transform();
	exitButtonObject->SetElement(exitButtonTransform, ElementType::Transform);

	Button* exitButton = new Button(770, 860, 770 + 381, 860 + 111);
	exitButton->SetOnClick([=]()
		{
			PostQuitMessage(0);
		});
	exitButtonObject->SetElement(exitButton, ElementType::Button);

	AudioSource* exitButtonAudioSource = new AudioSource(m_gameEngine);
	exitButtonObject->SetElement(exitButtonAudioSource, ElementType::AudioSource);
	exitButton->SetAudioSource(exitButtonAudioSource);
	exitButtonAudioSource->AddSoundVector("sfx_OnButton");
	exitButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion
#pragma region OptionButton
#pragma region SoundPopup
	/// 옵션UI생성
	/// OptionUIObject 생성
	GameObject* SoundOptionUIObject = m_gameEngine->CreateObject("optionUI", thisScene);
	Transform* soundOptionUITransform = new Transform;
	SoundOptionUIObject->SetElement(soundOptionUITransform, ElementType::Transform);
	soundOptionUITransform->SetLayer(8);
	soundOptionUITransform->SetLocalPosition({ (1920 / 2) - 315.5f ,(1080 / 2) - 215.5f });

	// 볼륨슬라이더 오브젝트 생성
	GameObject* MasterVolumeSliderObject = m_gameEngine->CreateObject("MsterVolumeSlider", thisScene);
	GameObject* BGMVolumeSliderObject = m_gameEngine->CreateObject("BGMVolumeSlider", thisScene);
	GameObject* SFXVolumeSliderObject = m_gameEngine->CreateObject("SFXVolumeSlider", thisScene);

	// 슬라이더 트랜스폼
	Transform* masterVolumeSliderTransform = new Transform;
	Transform* BGMVolumeSliderTransform = new Transform;
	Transform* SFXVolumeSliderTransform = new Transform;

	MasterVolumeSliderObject->SetElement(masterVolumeSliderTransform, ElementType::Transform);
	BGMVolumeSliderObject->SetElement(BGMVolumeSliderTransform, ElementType::Transform);
	SFXVolumeSliderObject->SetElement(SFXVolumeSliderTransform, ElementType::Transform);

	masterVolumeSliderTransform->SetParent(soundOptionUITransform);
	BGMVolumeSliderTransform->SetParent(soundOptionUITransform);
	SFXVolumeSliderTransform->SetParent(soundOptionUITransform);

	masterVolumeSliderTransform->SetLayer(9);
	BGMVolumeSliderTransform->SetLayer(9);
	SFXVolumeSliderTransform->SetLayer(9);

	// 위치를 미리 찍는다.
	masterVolumeSliderTransform->SetLocalPosition({ 160, 100 });
	BGMVolumeSliderTransform->SetLocalPosition({ 160, 200 });
	SFXVolumeSliderTransform->SetLocalPosition({ 160, 300 });

	// 볼륨슬라이더 리소스 생성
	// (슬라이더 리소스들의 해제가 이루어지지 않고 있음)
	// ui
	Graphics::Sprite* soundOptionUIBackground = m_directY->ReadSpriteTxtFile("sounduibar", m_gameEngine->GetImage("sounduibar"));
	// slider
	Graphics::Sprite* sliderBackground_Master = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_BGM = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_SFX = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderFillArea_Master = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_Master = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_BGM = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_BGM = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_SFX = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_SFX = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));

	Graphics::Sprite* BGImage = m_directY->ReadSpriteTxtFile("bgsound", m_gameEngine->GetImage("bgsound")); //배경음 글자 이미지
	Graphics::Sprite* EffectImage = m_directY->ReadSpriteTxtFile("effectsound", m_gameEngine->GetImage("effectsound")); //효과음 글자 이미지
	Graphics::Sprite* MasterImage = m_directY->ReadSpriteTxtFile("mastersound", m_gameEngine->GetImage("mastersound")); //마스터 글자 이미지

	// 슬라이더 AA
	Slider* masterVolumeSlider = new Slider((int)sliderBackground_Master->m_drawSize.left, (int)sliderBackground_Master->m_drawSize.right,
		(int)sliderBackground_Master->m_drawSize.top, (int)sliderBackground_Master->m_drawSize.bottom,
		(int)(sliderBackground_Master->m_drawSize.left + sliderBackground_Master->m_drawSize.right) / 2,
		(int)(sliderBackground_Master->m_drawSize.top, sliderBackground_Master->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(MASTER));
	Slider* SFXVolumeSlider = new Slider((int)sliderBackground_SFX->m_drawSize.left, (int)sliderBackground_SFX->m_drawSize.right,
		(int)sliderBackground_SFX->m_drawSize.top, (int)sliderBackground_SFX->m_drawSize.bottom,
		(int)(sliderBackground_SFX->m_drawSize.left + sliderBackground_SFX->m_drawSize.right) / 2,
		(int)(sliderBackground_SFX->m_drawSize.top, sliderBackground_SFX->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(SFX));
	Slider* BGMVolumeSlider = new Slider((int)sliderBackground_BGM->m_drawSize.left, (int)sliderBackground_BGM->m_drawSize.right,
		(int)sliderBackground_BGM->m_drawSize.top, (int)sliderBackground_BGM->m_drawSize.bottom,
		(int)(sliderBackground_BGM->m_drawSize.left + sliderBackground_BGM->m_drawSize.right) / 2,
		(int)(sliderBackground_BGM->m_drawSize.top, sliderBackground_BGM->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(BGM));



	// 슬라이더 이미지 저장
	masterVolumeSlider->SetBackground(sliderBackground_Master);
	masterVolumeSlider->SetFillArea(sliderFillArea_Master);
	masterVolumeSlider->SetHandle(sliderBar_Master);

	BGMVolumeSlider->SetBackground(sliderBackground_BGM);
	BGMVolumeSlider->SetFillArea(sliderFillArea_BGM);
	BGMVolumeSlider->SetHandle(sliderBar_BGM);

	SFXVolumeSlider->SetBackground(sliderBackground_SFX);
	SFXVolumeSlider->SetFillArea(sliderFillArea_SFX);
	SFXVolumeSlider->SetHandle(sliderBar_SFX);

	MasterVolumeSliderObject->SetElement(masterVolumeSlider, ElementType::Slider);
	BGMVolumeSliderObject->SetElement(BGMVolumeSlider, ElementType::Slider);
	SFXVolumeSliderObject->SetElement(SFXVolumeSlider, ElementType::Slider);

	// 슬라이더 오브젝트만 가져서 수치 반영만 한다.
	SoundOptionUI* soundOptionUI = new SoundOptionUI(masterVolumeSlider, BGMVolumeSlider, SFXVolumeSlider);
	SoundOptionUIObject->SetElement(soundOptionUI, ElementType::Script);
	soundOptionUI->SetUISprite(soundOptionUIBackground);
	soundOptionUI->SetMasterTiltle(MasterImage);
	soundOptionUI->SetBgmTiltle(BGImage);
	soundOptionUI->SetSfxTiltle(EffectImage);

	soundOptionUI->Init();

	/// 사운드 옵션 끄기 버튼
	GameObject* soundExitButtonObject = m_gameEngine->CreateObject("soundExitbutton", thisScene);
	Transform* soundExitbuttonTransform = new Transform();
	soundExitButtonObject->SetElement(soundExitbuttonTransform, ElementType::Transform);
	soundExitbuttonTransform->SetLocalPosition({ 597, 0 });
	soundExitbuttonTransform->SetLayer(500);
	soundExitButtonObject->SetActive(false);
	soundExitButtonObject->SetIsDraw(false);

	Button* soundExitButton = new Button(1240, 325, 1275, 360);
	Graphics::Sprite* soundExiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	soundExitButton->m_sprite = soundExiteButtonImage;
	soundExitButtonObject->SetElement(soundExitButton, ElementType::Button);

	AudioSource* soundExitButtonAudioSource = new AudioSource(m_gameEngine);
	soundExitButtonObject->SetElement(soundExitButtonAudioSource, ElementType::AudioSource);
	soundExitButton->SetAudioSource(soundExitButtonAudioSource);
	soundExitButtonAudioSource->AddSoundVector("sfx_OnButton");
	soundExitButtonAudioSource->AddSoundVector("sfx_StageSelect_click");


#pragma endregion SoundPopup

	// 사운드 (버튼)
	GameObject* optionButtonObject = m_gameEngine->CreateObject("OptionButton", thisScene);

	Transform* optionButtonTransform = new Transform();
	optionButtonObject->SetElement(optionButtonTransform, ElementType::Transform);

	Button* optionButton = new Button(57, 947, 57 + 81, 947 + 81);
	optionButtonObject->SetElement(optionButton, ElementType::Button);

	AudioSource* optionButtonAudioSource = new AudioSource(m_gameEngine);
	optionButtonObject->SetElement(optionButtonAudioSource, ElementType::AudioSource);
	optionButton->SetAudioSource(optionButtonAudioSource);
	optionButtonAudioSource->AddSoundVector("sfx_OnButton");
	optionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");
#pragma endregion
#pragma region SecretButton
	// 게임시작 (버튼)
	GameObject* secretButtonObject = m_gameEngine->CreateObject("SecretButton", thisScene);

	Transform* secretButtonTransform = new Transform();
	secretButtonObject->SetElement(secretButtonTransform, ElementType::Transform);

	Button* secretButton = new Button(1250, 135, 1595, 270);
	secretButton->SetOnClick([=]()
		{
			m_gameEngine->SetNextScene("SecretScene");
		});
	secretButtonObject->SetElement(secretButton, ElementType::Button);

	AudioSource* secretButtonAudiosource = new AudioSource(m_gameEngine);
	secretButtonObject->SetElement(secretButtonAudiosource, ElementType::AudioSource);
	secretButton->SetAudioSource(secretButtonAudiosource);
	secretButtonAudiosource->AddSoundVector("sfx_OnButton");
	secretButtonAudiosource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion
	dataResetExitButton->SetOnClick([=]()
		{
			dataResetPopupObject->SetActive(false);

			// 타이틀 버튼
			startButtonObject->SetActive(true);
			dataResetButtonObject->SetActive(true);
			exitButtonObject->SetActive(true);
		});
	dataResetButton->SetOnClick([=]()
		{
			dataResetPopupObject->SetActive(true);
			ClearManager::GetInstance().SetClearStage(0);

			// 타이틀 버튼
			startButtonObject->SetActive(false);
			dataResetButtonObject->SetActive(false);
			exitButtonObject->SetActive(false);
		});

	soundExitButton->SetOnClick([=]()
		{
			// 타이틀 버튼
			startButtonObject->SetActive(true);
			dataResetButtonObject->SetActive(true);
			exitButtonObject->SetActive(true);

			// 사운드 팝업 관련 오브젝트
			soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
			SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetActive());
			MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
			SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
			BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
		});

	optionButton->SetOnClick([=]()
		{
			// 타이틀 버튼
			startButtonObject->SetActive(false);
			dataResetButtonObject->SetActive(false);
			exitButtonObject->SetActive(false);

			// 사운드 팝업 관련 오브젝트
			soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
			SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetIsDraw());
			MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
			SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
			BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
		});

#pragma endregion UI

}

void GameProcess::InitOpeningScene()
{
	Scene* thisScene = m_gameEngine->FindScene("OpeningScene");
	m_gameEngine->LoadResource("OpeningScene", m_directY);

	m_gameEngine->SetTimeScale(1);

	/// 테스트용 bgm재생	
	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("bgm_cutscene_opening_loop"), 1.f, PlayType::BGM);

#pragma region DialogManager
	GameObject* dialogManagerObject = m_gameEngine->CreateObject("DialogManager", thisScene);
	Transform* dialogManagerTransform = new Transform();
	dialogManagerObject->SetElement(dialogManagerTransform, ElementType::Transform);
	dialogManagerTransform->SetUseCamera(false);
	dialogManagerTransform->SetLayer(10000);

	Graphics::Sprite* dialogImage = m_directY->ReadSpriteTxtFile("dialog", m_gameEngine->GetImage("dialog"));
	dialogImage->m_drawPos = { 960 - dialogImage->m_drawSize.right / 2, 775 };

	Graphics::Sprite* dialogGodImage = m_directY->ReadSpriteTxtFile("dialogGod", m_gameEngine->GetImage("dialogGod"));
	dialogGodImage->m_drawPos = { 960 - dialogGodImage->m_drawSize.right / 2, 760 };

	DialogManager* dialogManager = new DialogManager(0, 0, 1396, 1080, 9000, dialogImage, dialogGodImage);
	dialogManagerObject->SetElement(dialogManager, ElementType::Script);
	dialogManager->ReadDialogCSV("Resource/Dialog2");
	dialogManager->SetTextOffset({ 130 + 960 - dialogGodImage->m_drawSize.right / 2, 860 });
	dialogManager->SetStartGame([=]() 
		{
			m_gameEngine->SetNextScene("StageScene");
		});

	AudioSource* dialogManagerAudioSource = new AudioSource(m_gameEngine);
	dialogManagerObject->SetElement(dialogManagerAudioSource, ElementType::AudioSource);
	dialogManager->SetAudioSource(dialogManagerAudioSource);
	dialogManagerAudioSource->AddSound("sfx_dialog_1");

	//BGI 로 변경
	Graphics::Sprite* openingCutScene1 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_1"));
	dialogManager->AddSprite("Opening_1", openingCutScene1);
	Graphics::Sprite* openingCutScene2 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_2"));
	dialogManager->AddSprite("Opening_2", openingCutScene2);
	Graphics::Sprite* openingCutScene3 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_3"));
	dialogManager->AddSprite("Opening_3", openingCutScene3);
	Graphics::Sprite* openingCutScene4 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_4"));
	dialogManager->AddSprite("Opening_4", openingCutScene4);
	Graphics::Sprite* openingCutScene5 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_5"));
	dialogManager->AddSprite("Opening_5", openingCutScene5);
	Graphics::Sprite* openingCutScene6 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_6"));
	dialogManager->AddSprite("Opening_6", openingCutScene6);
	Graphics::Sprite* openingCutScene7 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_7"));
	dialogManager->AddSprite("Opening_7", openingCutScene7);
	Graphics::Sprite* openingCutScene8 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_8"));
	dialogManager->AddSprite("Opening_8", openingCutScene8);
	Graphics::Sprite* openingCutScene9 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Opening_9"));
	dialogManager->AddSprite("Opening_9", openingCutScene9);

#pragma endregion DialogManager
}

void GameProcess::InitStageScene()
{
	Scene* thisScene = m_gameEngine->FindScene("StageScene");
	m_gameEngine->LoadResource("StageScene", m_directY);

	m_gameEngine->SetTimeScale(1);

	/// 테스트용 bgm재생	
	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("bgm_title_loop"), 1.f, PlayType::BGM);

#pragma region UI

#pragma region Lock
	GameObject* lockObject = m_gameEngine->CreateObject("Lock", thisScene);

	Transform* lockTransform = new Transform();
	lockObject->SetElement(lockTransform, ElementType::Transform);
	lockTransform->SetLayer(20000);

	Graphics::Sprite* lockImage = m_directY->ReadSpriteTxtFile("lock", m_gameEngine->GetImage("lock"));
	Graphics::Sprite* clearImage = m_directY->ReadSpriteTxtFile("lock", m_gameEngine->GetImage("clearText"));
	LockStage* lock = new LockStage(lockImage, clearImage);
	lockObject->SetElement(lock, ElementType::Script);
#pragma endregion 

#pragma region backgroundImage
	GameObject* backgroundImageObject = m_gameEngine->CreateObject("BackgroundImage", thisScene);

	Transform* backgroundImageTransform = new Transform();
	backgroundImageObject->SetElement(backgroundImageTransform, ElementType::Transform);

	Graphics::Sprite* backButtonImageSprite = m_directY->ReadSpriteTxtFile("stageBackground", m_gameEngine->GetImage("stageBackground"));
	BackgroundImage* backgroundImage = new BackgroundImage(backButtonImageSprite);
	backgroundImageObject->SetElement(backgroundImage, ElementType::Button);
#pragma endregion 

#pragma region StageTitle
	GameObject* stageTitleObject = m_gameEngine->CreateObject("StageTitle", thisScene);

	Transform* stageTitleTransform = new Transform();
	stageTitleObject->SetElement(stageTitleTransform, ElementType::Transform);

	Graphics::Sprite* stageTitleSprite1 = m_directY->ReadSpriteTxtFile("stageTitle", m_gameEngine->GetImage("title1"));
	Graphics::Sprite* stageTitleSprite2 = m_directY->ReadSpriteTxtFile("stageTitle", m_gameEngine->GetImage("title2"));
	Graphics::Sprite* stageTitleSprite3 = m_directY->ReadSpriteTxtFile("stageTitle", m_gameEngine->GetImage("title3"));

	stageTitleTransform->SetLocalPosition({ 960 - stageTitleSprite1->m_drawSize.right / 2, 60 });

	StageTitle* stageTitle = new StageTitle(stageTitleSprite1, stageTitleSprite2, stageTitleSprite3);
	stageTitleObject->SetElement(stageTitle, ElementType::Script);
#pragma endregion 

#pragma region BackButton
	GameObject* backButtonObject = m_gameEngine->CreateObject("BackButton", thisScene);

	Transform* backButtonTransform = new Transform();
	backButtonObject->SetElement(backButtonTransform, ElementType::Transform);
	backButtonTransform->SetLocalPosition({ 30, 30 });

	// 생성자 값 다시 넣어야 함
	Button* backButton = new Button(30, 30, 83, 83);
	Graphics::Sprite* backButtonImage = m_directY->ReadSpriteTxtFile("stageBack", m_gameEngine->GetImage("stageBack"));
	backButton->m_sprite = backButtonImage;

	backButton->SetOnClick([=]()
		{
			m_gameEngine->SetNextScene("TitleScene");
		});
	backButtonObject->SetElement(backButton, ElementType::Button);

	AudioSource* backButtonAudioSource = new AudioSource(m_gameEngine);
	backButtonObject->SetElement(backButtonAudioSource, ElementType::AudioSource);
	backButton->SetAudioSource(backButtonAudioSource);
	backButtonAudioSource->AddSoundVector("sfx_OnButton");
	backButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion 

#pragma region Stage1Button
	GameObject* stage1ButtonObject = m_gameEngine->CreateObject("Stage1Button", thisScene);

	Transform* stage1ButtonTransform = new Transform();
	stage1ButtonObject->SetElement(stage1ButtonTransform, ElementType::Transform);

	Button* stage1Button = new Button(60, 214, 631, 1115);
	Graphics::Sprite* stage1ButtonImage = m_directY->ReadSpriteTxtFile("stage", m_gameEngine->GetImage("stage1"));
	stage1Button->m_sprite = stage1ButtonImage;
	stage1Button->SetOnClick([=]()
		{
			m_gameEngine->SetNextScene("GameScene1");
		});
	stage1ButtonObject->SetElement(stage1Button, ElementType::Button);

	AudioSource* stage1buttonAudioSource = new AudioSource(m_gameEngine);
	stage1ButtonObject->SetElement(stage1buttonAudioSource, ElementType::AudioSource);
	stage1Button->SetAudioSource(stage1buttonAudioSource);
	stage1buttonAudioSource->AddSoundVector("sfx_OnButton");
	stage1buttonAudioSource->AddSoundVector("sfx_StageSelect_click");

	GameObject* stage1ImageObject = m_gameEngine->CreateObject("Stage1Image", thisScene);

	Transform* stage1ImageTransform = new Transform();
	stage1ImageObject->SetElement(stage1ImageTransform, ElementType::Transform);
	stage1ImageTransform->SetLocalPosition({ 60 + 50, 259 });

	Graphics::Sprite* stage1ImageSprite = m_directY->ReadSpriteTxtFile("lock", m_gameEngine->GetImage("stageImage1"));
	BackgroundImage* stage1Image = new BackgroundImage(stage1ImageSprite);
	stage1ImageObject->SetElement(stage1Image, ElementType::Script);
#pragma endregion 

#pragma region Stage2Button
	GameObject* stage2ButtonObject = m_gameEngine->CreateObject("Stage2Button", thisScene);

	Transform* stage2ButtonTransform = new Transform();
	stage2ButtonObject->SetElement(stage2ButtonTransform, ElementType::Transform);

	Button* stage2Button = new Button(676, 214, 676 + 571, 1115);
	Graphics::Sprite* stage2ButtonImage = m_directY->ReadSpriteTxtFile("stage", m_gameEngine->GetImage("stage2"));
	stage2Button->m_sprite = stage2ButtonImage;
	stage2Button->SetOnClick([=]()
		{
			if (ClearManager::GetInstance().GetClearStage() >= 1)
			{
				m_gameEngine->SetNextScene("GameScene2");
			}
		});
	stage2ButtonObject->SetElement(stage2Button, ElementType::Button);

	AudioSource* stage2buttonAudioSource = new AudioSource(m_gameEngine);
	stage2ButtonObject->SetElement(stage2buttonAudioSource, ElementType::AudioSource);
	stage2Button->SetAudioSource(stage2buttonAudioSource);
	stage2buttonAudioSource->AddSoundVector("sfx_OnButton");
	stage2buttonAudioSource->AddSoundVector("sfx_StageSelect_click");

	GameObject* stage2ImageObject = m_gameEngine->CreateObject("Stage2Image", thisScene);

	Transform* stage2ImageTransform = new Transform();
	stage2ImageObject->SetElement(stage2ImageTransform, ElementType::Transform);
	stage2ImageTransform->SetLocalPosition({ 676 + 50, 259 });

	Graphics::Sprite* stage2ImageSprite = m_directY->ReadSpriteTxtFile("lock", m_gameEngine->GetImage("stageImage2"));
	BackgroundImage* stage2Image = new BackgroundImage(stage2ImageSprite);
	stage2ImageObject->SetElement(stage2Image, ElementType::Script);
#pragma endregion 

#pragma region Stage3Button
	GameObject* stage3ButtonObject = m_gameEngine->CreateObject("Stage3Button", thisScene);

	Transform* stage3ButtonTransform = new Transform();
	stage3ButtonObject->SetElement(stage3ButtonTransform, ElementType::Transform);

	Button* stage3Button = new Button(1292, 214, 1292 + 571, 1115);
	Graphics::Sprite* stage3ButtonImage = m_directY->ReadSpriteTxtFile("stage", m_gameEngine->GetImage("stage3"));
	stage3Button->m_sprite = stage3ButtonImage;
	stage3Button->SetOnClick([=]()
		{
			if (ClearManager::GetInstance().GetClearStage() >= 2)
			{
				m_gameEngine->SetNextScene("GameScene3");
			}
		});
	stage3ButtonObject->SetElement(stage3Button, ElementType::Button);

	AudioSource* stage3buttonAudioSource = new AudioSource(m_gameEngine);
	stage3ButtonObject->SetElement(stage3buttonAudioSource, ElementType::AudioSource);
	stage3Button->SetAudioSource(stage3buttonAudioSource);
	stage3buttonAudioSource->AddSoundVector("sfx_OnButton");
	stage3buttonAudioSource->AddSoundVector("sfx_StageSelect_click");

	GameObject* stage3ImageObject = m_gameEngine->CreateObject("Stage3Image", thisScene);

	Transform* stage3ImageTransform = new Transform();
	stage3ImageObject->SetElement(stage3ImageTransform, ElementType::Transform);
	stage3ImageTransform->SetLocalPosition({ 1292 + 50, 259 });

	Graphics::Sprite* stage3ImageSprite = m_directY->ReadSpriteTxtFile("lock", m_gameEngine->GetImage("stageImage3"));
	BackgroundImage* stage3Image = new BackgroundImage(stage3ImageSprite);
	stage3ImageObject->SetElement(stage3Image, ElementType::Script);
#pragma endregion 

#pragma endregion UI
}

void GameProcess::InitGameOverScene()
{
	Scene* thisScene = m_gameEngine->FindScene("GameOverScene");
	m_gameEngine->LoadResource("GameOverScene", m_directY);

	m_gameEngine->SetTimeScale(1);

	/// 테스트용 bgm재생	
	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("sfx_gameover")/*, 1.f, PlayType::BGM*/);

#pragma region BackgroundImage
	// 타이틀 (이미지)
	GameObject* backgroundImageObject = m_gameEngine->CreateObject("BackgroundImage", thisScene);

	Transform* backgroundImageTransform = new Transform();
	backgroundImageObject->SetElement(backgroundImageTransform, ElementType::Transform);

	Graphics::Sprite* gameOverImage = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("gameOver"));
	BackgroundImage* backgroundImage = new BackgroundImage(gameOverImage);
	backgroundImageObject->SetElement(backgroundImage, ElementType::Script);
#pragma endregion
#pragma region MainButton
// 게임시작 (버튼)
	GameObject* mainButtonObject = m_gameEngine->CreateObject("MainButton", thisScene);

	Transform* mainButtonTransform = new Transform();
	mainButtonObject->SetElement(mainButtonTransform, ElementType::Transform);

	Button* mainButton = new Button(1408, 725, 1820, 835);
	mainButton->SetOnClick([=]()
		{
			m_gameEngine->SetNextScene("TitleScene");
		});
	mainButtonObject->SetElement(mainButton, ElementType::Button);

	AudioSource* mainButtonAudioSource = new AudioSource(m_gameEngine);
	mainButtonObject->SetElement(mainButtonAudioSource, ElementType::AudioSource);
	mainButton->SetAudioSource(mainButtonAudioSource);
	mainButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion
#pragma region RestartButton
	// 다시시작 (버튼)
	GameObject* restartButtonObject = m_gameEngine->CreateObject("RestartButton", thisScene);

	Transform* restartButtonTransform = new Transform();
	restartButtonObject->SetElement(restartButtonTransform, ElementType::Transform);

	Button* restartButton = new Button(1408, 870, 1820, 980);
	restartButton->SetOnClick([=]()
		{
			m_gameEngine->SetNextScene(m_gameEngine->GetPreScene()->GetName());
		});
	restartButtonObject->SetElement(restartButton, ElementType::Button);

	AudioSource* restartButtonAudioSource = new AudioSource(m_gameEngine);
	restartButtonObject->SetElement(restartButtonAudioSource, ElementType::AudioSource);
	restartButton->SetAudioSource(restartButtonAudioSource);
	restartButtonAudioSource->AddSoundVector("sfx_OnButton");
	restartButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion
}

void GameProcess::InitGameClearScene()
{
	//SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("sfx_gameover")/*, 1.f, PlayType::BGM*/);
	
	Scene* thisScene = m_gameEngine->FindScene("GameClearScene");
	m_gameEngine->LoadResource("GameClearScene", m_directY);

	m_gameEngine->SetTimeScale(1);

	/// 테스트용 bgm재생	
	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("bgm_ending")/*, 1.f, PlayType::BGM*/);

#pragma region DialogManager
	GameObject* dialogManagerObject = m_gameEngine->CreateObject("DialogManager", thisScene);
	Transform* dialogManagerTransform = new Transform();
	dialogManagerObject->SetElement(dialogManagerTransform, ElementType::Transform);
	dialogManagerTransform->SetUseCamera(false);
	dialogManagerTransform->SetLayer(10000);

	Graphics::Sprite* dialogImage = m_directY->ReadSpriteTxtFile("dialog", m_gameEngine->GetImage("dialog"));
	dialogImage->m_drawPos = { 960 - dialogImage->m_drawSize.right / 2, 775 };

	Graphics::Sprite* dialogGodImage = m_directY->ReadSpriteTxtFile("dialogGod", m_gameEngine->GetImage("dialogGod"));
	dialogGodImage->m_drawPos = { 960 - dialogGodImage->m_drawSize.right / 2, 760 };

	DialogManager* dialogManager = new DialogManager(250, 0, 1920 - 250, 1080, 10000, dialogImage, dialogGodImage);
	dialogManagerObject->SetElement(dialogManager, ElementType::Script);
	dialogManager->ReadDialogCSV("Resource/Dialog2");
	dialogManager->SetTextOffset({ 130 + 960 - dialogGodImage->m_drawSize.right / 2, 860 });
	dialogManager->SetStartGame([=]()
		{
			m_gameEngine->SetNextScene("GameCreditScene");
		});

	AudioSource* dialogManagerAudioSource = new AudioSource(m_gameEngine);
	dialogManagerObject->SetElement(dialogManagerAudioSource, ElementType::AudioSource);
	dialogManager->SetAudioSource(dialogManagerAudioSource);
	dialogManagerAudioSource->AddSound("sfx_dialog_1");

	//BGI 로 변경
	Graphics::Sprite* endingCutScene1 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Ending_1"));
	dialogManager->AddSprite("Ending_1", endingCutScene1);
	Graphics::Sprite* endingCutScene2 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Ending_2"));
	dialogManager->AddSprite("Ending_2", endingCutScene2);
	Graphics::Sprite* endingCutScene3 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Ending_3"));
	dialogManager->AddSprite("Ending_3", endingCutScene3);

	Graphics::Sprite* goddessImageSprite = m_directY->ReadSpriteTxtFile("goddessStanding", m_gameEngine->GetImage("goddess"));
	Graphics::Sprite* goddessDarkImageSprite = m_directY->ReadSpriteTxtFile("goddessStanding", m_gameEngine->GetImage("goddess_Dark"));
	Graphics::Sprite* playerImageSprite = m_directY->ReadSpriteTxtFile("playerStanding", m_gameEngine->GetImage("Player"));
	Graphics::Sprite* playerDarkImageSprite = m_directY->ReadSpriteTxtFile("playerStanding", m_gameEngine->GetImage("Player_Dark"));

	dialogManager->AddSprite("goddess", goddessImageSprite);
	dialogManager->AddSprite("goddess_Dark", goddessDarkImageSprite);
	dialogManager->AddSprite("Player", playerImageSprite);
	dialogManager->AddSprite("Player_Dark", playerDarkImageSprite);
#pragma endregion DialogManager
#pragma endregion
}

void GameProcess::InitGameCreditScene()
{
	Scene* thisScene = m_gameEngine->FindScene("GameCreditScene");
	m_gameEngine->LoadResource("GameCreditScene", m_directY);

	m_gameEngine->SetTimeScale(1);

#pragma region Credit
	GameObject* creditObject = m_gameEngine->CreateObject("Credit", thisScene);

	Transform* creditTransform = new Transform();
	creditObject->SetElement(creditTransform, ElementType::Transform);
	
	Graphics::Sprite* background = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits_Background"));
	Graphics::Sprite* truck = m_directY->ReadSpriteTxtFile("truckCredit", m_gameEngine->GetImage("Credits_Truck"));

	Credit* credit = new Credit(background, truck, 9);
	creditObject->SetElement(credit, ElementType::Script);

	Graphics::Sprite* credit1 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.001"));
	credit->AddSprite(credit1);
	Graphics::Sprite* credit2 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.002"));
	credit->AddSprite(credit2);
	Graphics::Sprite* credit3 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.003"));
	credit->AddSprite(credit3);
	Graphics::Sprite* credit4 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.004"));
	credit->AddSprite(credit4);
	Graphics::Sprite* credit5 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.005"));
	credit->AddSprite(credit5);
	Graphics::Sprite* credit6 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.006"));
	credit->AddSprite(credit6);
	Graphics::Sprite* credit7 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.007"));
	credit->AddSprite(credit7);
	Graphics::Sprite* credit8 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.008"));
	credit->AddSprite(credit8);
	Graphics::Sprite* credit9 = m_directY->ReadSpriteTxtFile("title", m_gameEngine->GetImage("Credits.009"));
	credit->AddSprite(credit9);
#pragma endregion Credit

#pragma region NpcManager
	GameObject* NpcManagerObject = m_gameEngine->CreateObject("NpcManager", thisScene);
	Transform* NpcManagerTransform = new Transform();
	NpcManagerObject->SetElement(NpcManagerTransform, ElementType::Transform);
	NpcManagerTransform->SetUseCamera(true);
	NpcManager* npcManager = new NpcManager(m_gameEngine, thisScene, m_directY);
	NpcManagerObject->SetElement(npcManager, ElementType::Script);
	npcManager->Init();
	npcManager->ReadSource("Resource/Text/Map/Stage1_npcLayer.txt");
	//npcManager->SetPrintLog([=](std::wstring log, bool isNpc) { scrollbar->AddText(log, isNpc); });
	//npcManager->MakeNpc();
	npcManager->MakeNpcObjectInCredit();

	std::vector<SkinHeadNPC*> npcs = npcManager->GetNpcs();
	for (int i = 0; i < npcs.size(); i++)
	{
		npcs[i]->GetGameObject()->SetActive(false);
	}
#pragma endregion NpcManager

	credit->SetNPCFly([=]() 
		{
			for (int i = 0; i < npcs.size(); i++)
			{
				GameObject* npc = npcs[i]->GetGameObject();
				npc->SetActive(true);
				npcs[i]->SetIsCreditFly(true);
			}
		});
	credit->SetGoTitle([=]()
		{
			for (int i = 0; i < npcs.size(); i++)
			{
				m_gameEngine->SetNextScene("TitleScene");
			}
		});

}

void GameProcess::InitSecretScene()
{
	Scene* thisScene = m_gameEngine->FindScene("SecretScene");
	m_gameEngine->LoadResource("GameScene", m_directY);

	m_gameEngine->SetTimeScale(1);

	SoundManager::GetInstance().PlayBack(m_gameEngine->GetSound("bgm_maple"), 1, PlayType::BGM);

#pragma region UI

#pragma region MainUI
	/// MainUIWindow
	GameObject* UIObject = m_gameEngine->CreateObject("UI", thisScene);
	Transform* uiTransform = new Transform();
	UIObject->SetElement(uiTransform, ElementType::Transform);
	uiTransform->SetLocalPosition({ 1398,40 });
	uiTransform->SetLayer(300);

	InGameUI* mainUI = new InGameUI();
	UIObject->SetElement(mainUI, ElementType::Script);
	Graphics::Sprite* UIImage = m_directY->ReadSpriteTxtFile("ingame_mainsolid", m_gameEngine->GetImage("ingame_mainsolid"));
	mainUI->m_sprite = UIImage;
#pragma endregion MainUI

#pragma region HeroUI
	///면접자 초상화UI
	GameObject* heroPotraitObject = m_gameEngine->CreateObject("heroPotrait", thisScene);
	Transform* heroPotraitTransform = new Transform();
	heroPotraitObject->SetElement(heroPotraitTransform, ElementType::Transform);
	heroPotraitTransform->SetParent(uiTransform);
	heroPotraitTransform->Translate({ 25,446 });
	heroPotraitTransform->SetLayer(312);

	InterviewUI* heroPotraitUI = new InterviewUI();
	heroPotraitObject->SetElement(heroPotraitUI, ElementType::Script);
	Graphics::Motion* interviewImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("heroPotrait", m_gameEngine->GetImage("heroPotrait"), interviewImage);
	heroPotraitUI->SetImage(interviewImage);

	///면접자 얼굴 이미지
	GameObject* heroFaceObject = m_gameEngine->CreateObject("heroPotrait", thisScene);
	Transform* heroFaceTransform = new Transform();
	heroFaceObject->SetElement(heroFaceTransform, ElementType::Transform);
	heroFaceTransform->SetParent(uiTransform);
	heroFaceTransform->Translate({ 35,446 });
	heroFaceTransform->SetLayer(313);

	PotraitUI* heroFaceUI = new PotraitUI();
	heroFaceObject->SetElement(heroFaceUI, ElementType::Script);
	Graphics::Motion* potraitImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("potraitGroup", m_gameEngine->GetImage("potraitGroup"), potraitImage);
	heroFaceUI->SetPotraitGroup(potraitImage);

	///면접자 초상화 프레임
	GameObject* heroFrameObject = m_gameEngine->CreateObject("heroframe", thisScene);
	Transform* heroFrameTransform = new Transform();
	heroFrameObject->SetElement(heroFrameTransform, ElementType::Transform);
	heroFrameTransform->SetParent(uiTransform);
	heroFrameTransform->SetLocalPosition({ 25,446 });
	heroFrameTransform->SetLayer(314);

	InGameUI* heroFrameUI = new InGameUI();
	heroFrameObject->SetElement(heroFrameUI, ElementType::Script);
	Graphics::Sprite* heroFrameImage = m_directY->ReadSpriteTxtFile("heroframe", m_gameEngine->GetImage("heroframe"));
	heroFrameUI->m_sprite = heroFrameImage;
#pragma endregion HeroUI

#pragma region GoddessUI
	///여신 초상화
	GameObject* goddessPotraitObject = m_gameEngine->CreateObject("goddessPotrait", thisScene);
	Transform* goddessPotraitTransform = new Transform();
	goddessPotraitObject->SetElement(goddessPotraitTransform, ElementType::Transform);
	goddessPotraitTransform->SetParent(uiTransform);
	goddessPotraitTransform->Translate({ 305,446 });
	goddessPotraitTransform->SetLayer(312);

	InterviewUI* goddesspotraitUI = new InterviewUI();
	goddessPotraitObject->SetElement(goddesspotraitUI, ElementType::Script);
	Graphics::Motion* goddessImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("goddessPotrait", m_gameEngine->GetImage("goddessPotrait"), goddessImage);
	goddesspotraitUI->SetImage(goddessImage);

	///여신 얼굴 이미지
	GameObject* goddessFaceObject = m_gameEngine->CreateObject("goddessPotrait", thisScene);
	Transform* goddessFaceTransform = new Transform();
	goddessFaceObject->SetElement(goddessFaceTransform, ElementType::Transform);
	goddessFaceTransform->SetParent(uiTransform);
	goddessFaceTransform->Translate({ 315,453 });
	goddessFaceTransform->SetLayer(313);

	InterviewUI* goddessFaceUI = new InterviewUI();
	goddessFaceObject->SetElement(goddessFaceUI, ElementType::Script);
	Graphics::Motion* goddessFaceImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("portrait", m_gameEngine->GetImage("portrait_godness"), goddessFaceImage);
	goddessFaceUI->SetImage(goddessFaceImage);

	///여신 초상화 프레임
	GameObject* goddessBarFrameObject = m_gameEngine->CreateObject("goddessbarframe", thisScene);
	Transform* goddessBarFrameTransform = new Transform();
	goddessBarFrameObject->SetElement(goddessBarFrameTransform, ElementType::Transform);
	goddessBarFrameTransform->SetParent(uiTransform);
	goddessBarFrameTransform->Translate({ 305,446 });
	goddessBarFrameTransform->SetLayer(314);

	InterviewUI* goddessBarFrameUI = new InterviewUI();
	goddessBarFrameObject->SetElement(goddessBarFrameUI, ElementType::Script);
	Graphics::Motion* goddessBarFrameImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("goddessbarframe", m_gameEngine->GetImage("goddessbarframe"), goddessBarFrameImage);
	goddessBarFrameUI->SetImage(goddessBarFrameImage);
#pragma endregion GoddessUI

#pragma region TimerUI
	///Timer
	GameObject* TimerObject = m_gameEngine->CreateObject("Timer", thisScene);
	Transform* TimerTransform = new Transform();
	TimerObject->SetElement(TimerTransform, ElementType::Transform);
	// 부모트랜스폼 설정
	// 트랜스폼 레이어 설정(일단 보일 수 있도록 높은 숫자 넣음)
	TimerTransform->SetLayer(314);
	Timer* timer = new Timer(m_gameEngine, m_directY);
	TimerObject->SetElement(timer, ElementType::Script);

	// 오디오 소스를 추가함
	AudioSource* timerAudioSource = new AudioSource(m_gameEngine);
	TimerObject->SetElement(timerAudioSource, ElementType::AudioSource);
	timer->SetAudioSource(timerAudioSource);
	timerAudioSource->AddSound("bgm_beep_slow");
	timerAudioSource->AddSound("sfx_timer_warning");

	///타이머 장식
	GameObject* secondBaseObject = m_gameEngine->CreateObject("secondHandBase", thisScene);
	Transform* secondBaseTransform = new Transform();
	secondBaseTransform->SetParent(uiTransform);
	secondBaseTransform->SetLocalPosition({ 205 , 622 });
	secondBaseObject->SetElement(secondBaseTransform, ElementType::Transform);
	secondBaseTransform->SetLayer(315);

	InGameUI* secondBaseUI = new InGameUI();
	secondBaseObject->SetElement(secondBaseUI, ElementType::Script);
	Graphics::Sprite* secondBaseImage = m_directY->ReadSpriteTxtFile("secondhand_base", m_gameEngine->GetImage("secondhand_base"));
	secondBaseUI->m_sprite = secondBaseImage;

	///타이머 프레임
	GameObject* clockbaseObject = m_gameEngine->CreateObject("clockbase", thisScene);
	Transform* clockbaseTransform = new Transform();
	clockbaseTransform->SetParent(uiTransform);
	clockbaseTransform->SetLocalPosition({ 120 , 652 });
	clockbaseObject->SetElement(clockbaseTransform, ElementType::Transform);
	clockbaseTransform->SetLayer(314);

	InGameUI* clockbaseUI = new InGameUI();
	clockbaseObject->SetElement(clockbaseUI, ElementType::Script);
	Graphics::Sprite* clockbaseImage = m_directY->ReadSpriteTxtFile("clockbase", m_gameEngine->GetImage("clockbase"));
	clockbaseUI->m_sprite = clockbaseImage;

#pragma endregion TimerUI

#pragma region MessageUI
	/// 메세지 창 Scrollbar
	GameObject* dialogueObject = m_gameEngine->CreateObject("Scrollbar", thisScene);
	Transform* dialogueTrasform = new Transform();
	dialogueObject->SetElement(dialogueTrasform, ElementType::Transform);
	dialogueTrasform->SetParent(uiTransform);
	dialogueTrasform->Translate({ 25, 35 });
	dialogueTrasform->SetLayer(310);

	Graphics::Sprite* bar = m_directY->ReadSpriteTxtFile("bar", m_gameEngine->GetImage("bar"));
	Graphics::Sprite* background = m_directY->ReadSpriteTxtFile("message", m_gameEngine->GetImage("message"));
	Graphics::Motion* herochatbubble = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("herochatwindow", m_gameEngine->GetImage("chatwindow"), herochatbubble);

	Graphics::Motion* goddesschatbubble = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("herochatwindow", m_gameEngine->GetImage("chatwindow_goddess"), goddesschatbubble);

	Scrollbar* scrollbar = new Scrollbar(35, 50, 422 - 35, 380 - 50, background, bar, herochatbubble, goddesschatbubble);
	dialogueObject->SetElement(scrollbar, ElementType::Scrollbar);
	scrollbar->SetHeroChatWindow(herochatbubble);
	scrollbar->SetGoddessChatWindow(goddesschatbubble);

	/*///메세지 프레임
	GameObject* messageFrameObject = m_gameEngine->CreateObject("heroframe", thisScene);
	Transform* messageFrameTransform = new Transform();
	messageFrameObject->SetElement(messageFrameTransform, ElementType::Transform);
	messageFrameTransform->SetParent(uiTransform);
	messageFrameTransform->SetLocalPosition({ 25,35 });
	messageFrameTransform->SetLayer(312);

	InGameUI* messageFrameUI = new InGameUI();
	messageFrameObject->SetElement(messageFrameUI, ElementType::Script);
	Graphics::Sprite* messageFrameImage = m_directY->ReadSpriteTxtFile("messageframe", m_gameEngine->GetImage("messageframe"));
	messageFrameUI->m_sprite = messageFrameImage;*/
#pragma endregion MessageUI

#pragma region InterviewRoomUI
	///면접장
	GameObject* InterviewRoomObject = m_gameEngine->CreateObject("interviewPlace", thisScene);
	Transform* InterviewRoomTransform = new Transform();
	InterviewRoomTransform->SetParent(uiTransform);
	InterviewRoomTransform->SetLocalPosition({ 36, 672 });
	InterviewRoomObject->SetElement(InterviewRoomTransform, ElementType::Transform);
	InterviewRoomTransform->SetLayer(310);

	InterviewUI* InterviewRoomUI = new InterviewUI();
	InterviewRoomObject->SetElement(InterviewRoomUI, ElementType::Script);
	Graphics::Motion* interviewPlaceImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_bar", m_gameEngine->GetImage("interview_screen_bar"), interviewPlaceImage);
	InterviewRoomUI->SetImage(interviewPlaceImage);

	/// 면접장 배경
	GameObject* InterviewRoomBGObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewRoomBGTransform = new Transform();
	InterviewRoomBGTransform->SetParent(uiTransform);
	InterviewRoomBGTransform->SetLocalPosition({ 35, 673 });
	InterviewRoomBGObject->SetElement(InterviewRoomBGTransform, ElementType::Transform);
	InterviewRoomBGTransform->SetLayer(311);

	InterviewUI* InterviewRoomBGUI = new InterviewUI();
	InterviewRoomBGObject->SetElement(InterviewRoomBGUI, ElementType::Script);
	Graphics::Motion* interviewRoomImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("InterviewRoomBackground", m_gameEngine->GetImage("InterviewRoomBackground"), interviewRoomImage);
	InterviewRoomBGUI->SetImage(interviewRoomImage);

	///면접장 빛
	GameObject* InterviewLightObject = m_gameEngine->CreateObject("interviewLight", thisScene);
	Transform* InterviewLightTransform = new Transform();
	InterviewLightTransform->SetParent(uiTransform);
	InterviewLightTransform->SetLocalPosition({ 35, 673 });
	InterviewLightObject->SetElement(InterviewLightTransform, ElementType::Transform);
	InterviewLightTransform->SetLayer(312);

	InterviewUI* InterviewLightUI = new InterviewUI();
	InterviewLightObject->SetElement(InterviewLightUI, ElementType::Script);
	Graphics::Motion* interviewLightImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interviewLight", m_gameEngine->GetImage("interviewLight"), interviewLightImage);
	InterviewLightUI->SetImage(interviewLightImage);

	/// 면접장 프레임
	GameObject* InterviewFrameObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewFrameTransform = new Transform();
	InterviewFrameTransform->SetParent(uiTransform);
	InterviewFrameTransform->SetLocalPosition({ 25, 535 });
	InterviewFrameObject->SetElement(InterviewFrameTransform, ElementType::Transform);
	InterviewFrameTransform->SetLayer(312);

	InterviewUI* InterviewFrameUI = new InterviewUI();
	InterviewFrameObject->SetElement(InterviewFrameUI, ElementType::Script);
	Graphics::Motion* interviewFrameImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_frame", m_gameEngine->GetImage("interview_screen_frame"), interviewFrameImage);
	InterviewFrameUI->SetImage(interviewFrameImage);

	///면접장 데코
	GameObject* InterviewDecoObject = m_gameEngine->CreateObject("interviewRoomBackGround", thisScene);
	Transform* InterviewDecoTransform = new Transform();
	InterviewDecoTransform->SetParent(uiTransform);
	InterviewDecoTransform->SetLocalPosition({ 20, 652 });
	InterviewDecoObject->SetElement(InterviewDecoTransform, ElementType::Transform);
	InterviewDecoTransform->SetLayer(313);

	InterviewUI* InterviewDecoUI = new InterviewUI();
	InterviewDecoObject->SetElement(InterviewDecoUI, ElementType::Script);
	Graphics::Motion* interviewDecoImage = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("interview_screen_frame_deco", m_gameEngine->GetImage("interview_screen_frame_deco"), interviewDecoImage);
	InterviewDecoUI->SetImage(interviewDecoImage);
#pragma endregion InterviewRoomUI

#pragma  region MainOptionUI

	GameObject* MainOptionUIObject = m_gameEngine->CreateObject("MainOptionUI", thisScene);
	Transform* MainOptionUITransform = new Transform;
	MainOptionUIObject->SetElement(MainOptionUITransform, ElementType::Transform);
	MainOptionUITransform->SetLayer(500);
	MainOptionUITransform->SetLocalPosition({ 960 - 315,540 - 215 });
	MainOptionUIObject->SetActive(false);

	MainOption* mainOptionUI = new MainOption();
	MainOptionUIObject->SetElement(mainOptionUI, ElementType::Script);
	Graphics::Sprite* mainOptionImage = m_directY->ReadSpriteTxtFile("menubar_all", m_gameEngine->GetImage("menubar_all"));
	mainOptionUI->SetImage(mainOptionImage);

	/// 타이틀로 가기 옵션 버튼
	GameObject* mainButtonObject = m_gameEngine->CreateObject("MainTitleButton", thisScene);
	mainButtonObject->SetActive(false);

	Transform* mainButtonTransform = new Transform();
	mainButtonObject->SetElement(mainButtonTransform, ElementType::Transform);
	mainButtonTransform->SetParent(MainOptionUITransform);
	mainButtonTransform->SetLocalPosition({ 185, 125 });

	Vector2 mainButtonWorldPosition = mainButtonTransform->GetWorldPosition();
	Button* mainButton = new Button(0 + (int)mainButtonWorldPosition.x, 0 + (int)mainButtonWorldPosition.y, 266 + (int)mainButtonWorldPosition.x, 71 + (int)mainButtonWorldPosition.y);
	mainButtonObject->SetElement(mainButton, ElementType::Script);
	mainButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("TitleScene");
			}
		});

	// 메인버튼 오디오소스
	AudioSource* mainButtonAudioSource = new AudioSource(m_gameEngine);
	mainButtonObject->SetElement(mainButtonAudioSource, ElementType::AudioSource);
	mainButton->SetAudioSource(mainButtonAudioSource);
	mainButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	/// 다시하기 옵션 버튼
	GameObject* replayButtonObject = m_gameEngine->CreateObject("ReplayButton", thisScene);
	replayButtonObject->SetActive(false);

	Transform* replayButtonTransform = new Transform();
	replayButtonObject->SetElement(replayButtonTransform, ElementType::Transform);
	replayButtonTransform->SetParent(MainOptionUITransform);
	replayButtonTransform->SetLocalPosition({ 185, 215 });

	Vector2 replaybuttonWorldPosition = replayButtonTransform->GetWorldPosition();
	Button* replayButton = new Button(0 + (int)replaybuttonWorldPosition.x, 0 + (int)replaybuttonWorldPosition.y, 266 + (int)replaybuttonWorldPosition.x, 71 + (int)replaybuttonWorldPosition.y);
	replayButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("SecretScene");
			}
		});
	replayButtonObject->SetElement(replayButton, ElementType::Script);

	// 다시하기 버튼 오디오소스
	AudioSource* replayButtonAudioSource = new AudioSource(m_gameEngine);
	replayButtonObject->SetElement(replayButtonAudioSource, ElementType::AudioSource);
	replayButton->SetAudioSource(replayButtonAudioSource);
	replayButtonAudioSource->AddSoundVector("sfx_OnButton");
	replayButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	///옵션 닫기
	GameObject* exitButtonObject = m_gameEngine->CreateObject("exitbutton", thisScene);

	Transform* exitbuttonTransform = new Transform();
	exitButtonObject->SetElement(exitbuttonTransform, ElementType::Transform);
	exitbuttonTransform->SetLayer(501);
	exitButtonObject->SetActive(false);

	Button* exitButton = new Button(1240, 325, 1276, 360);
	Graphics::Sprite* exiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	exitButton->m_sprite = exiteButtonImage;
	exitButtonObject->SetElement(exitButton, ElementType::Button);

	AudioSource* exitButtonAudioSource = new AudioSource(m_gameEngine);
	exitButtonObject->SetElement(exitButtonAudioSource, ElementType::AudioSource);
	exitButton->SetAudioSource(exitButtonAudioSource);
	exitButtonAudioSource->AddSoundVector("sfx_OnButton");
	exitButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

	/// 우상단 옵션 버튼
	GameObject* MainButtonObject = m_gameEngine->CreateObject("mainbutton", thisScene);
	Transform* MainButtonTransform = new Transform();
	MainButtonObject->SetElement(MainButtonTransform, ElementType::Transform);
	MainButtonTransform->SetLayer(500);

	Button* MainOptionButton = new Button(1865, 15, 1905, 55);
	Graphics::Sprite* MainButtonImage = m_directY->ReadSpriteTxtFile("button", m_gameEngine->GetImage("menubutton"));
	MainOptionButton->m_sprite = MainButtonImage;
	MainButtonObject->SetElement(MainOptionButton, ElementType::Button);

	AudioSource* mainOptionButtonAudioSource = new AudioSource(m_gameEngine);
	MainButtonObject->SetElement(mainOptionButtonAudioSource, ElementType::AudioSource);
	MainOptionButton->SetAudioSource(mainOptionButtonAudioSource);
	mainOptionButtonAudioSource->AddSoundVector("sfx_OnButton");
	mainOptionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion MainOptionUI	

#pragma region SoundOptionUI
	/// 옵션UI생성
	/// OptionUIObject 생성
	GameObject* SoundOptionUIObject = m_gameEngine->CreateObject("optionUI", thisScene);
	Transform* soundOptionUITransform = new Transform;
	SoundOptionUIObject->SetElement(soundOptionUITransform, ElementType::Transform);
	soundOptionUITransform->SetLayer(510);
	soundOptionUITransform->SetLocalPosition({ (1920 / 2) - 315.5f ,(1080 / 2) - 215.5f });

	// 볼륨슬라이더 오브젝트 생성
	GameObject* MasterVolumeSliderObject = m_gameEngine->CreateObject("MsterVolumeSlider", thisScene);
	GameObject* BGMVolumeSliderObject = m_gameEngine->CreateObject("BGMVolumeSlider", thisScene);
	GameObject* SFXVolumeSliderObject = m_gameEngine->CreateObject("SFXVolumeSlider", thisScene);

	// 슬라이더 트랜스폼
	Transform* masterVolumeSliderTransform = new Transform;
	Transform* BGMVolumeSliderTransform = new Transform;
	Transform* SFXVolumeSliderTransform = new Transform;

	MasterVolumeSliderObject->SetElement(masterVolumeSliderTransform, ElementType::Transform);
	BGMVolumeSliderObject->SetElement(BGMVolumeSliderTransform, ElementType::Transform);
	SFXVolumeSliderObject->SetElement(SFXVolumeSliderTransform, ElementType::Transform);

	masterVolumeSliderTransform->SetParent(soundOptionUITransform);
	BGMVolumeSliderTransform->SetParent(soundOptionUITransform);
	SFXVolumeSliderTransform->SetParent(soundOptionUITransform);

	masterVolumeSliderTransform->SetLayer(511);
	BGMVolumeSliderTransform->SetLayer(511);
	SFXVolumeSliderTransform->SetLayer(511);

	// 위치를 미리 찍는다.
	masterVolumeSliderTransform->SetLocalPosition({ 160, 100 });
	BGMVolumeSliderTransform->SetLocalPosition({ 160, 200 });
	SFXVolumeSliderTransform->SetLocalPosition({ 160, 300 });

	// 볼륨슬라이더 리소스 생성
	// (슬라이더 리소스들의 해제가 이루어지지 않고 있음)
	// ui
	Graphics::Sprite* soundOptionUIBackground = m_directY->ReadSpriteTxtFile("sounduibar", m_gameEngine->GetImage("sounduibar"));
	// slider
	Graphics::Sprite* sliderBackground_Master = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_BGM = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderBackground_SFX = m_directY->ReadSpriteTxtFile("soundbasebar", m_gameEngine->GetImage("soundbasebar"));
	Graphics::Sprite* sliderFillArea_Master = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_Master = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_BGM = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_BGM = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));
	Graphics::Sprite* sliderFillArea_SFX = m_directY->ReadSpriteTxtFile("soundcontrolbar", m_gameEngine->GetImage("soundcontrolbar"));
	Graphics::Sprite* sliderBar_SFX = m_directY->ReadSpriteTxtFile("soundcircle", m_gameEngine->GetImage("soundcircle"));

	Graphics::Sprite* BGImage = m_directY->ReadSpriteTxtFile("bgsound", m_gameEngine->GetImage("bgsound")); //배경음 글자 이미지
	Graphics::Sprite* EffectImage = m_directY->ReadSpriteTxtFile("effectsound", m_gameEngine->GetImage("effectsound")); //효과음 글자 이미지
	Graphics::Sprite* MasterImage = m_directY->ReadSpriteTxtFile("mastersound", m_gameEngine->GetImage("mastersound")); //마스터 글자 이미지

	// 슬라이더 AA
	Slider* masterVolumeSlider = new Slider((int)sliderBackground_Master->m_drawSize.left, (int)sliderBackground_Master->m_drawSize.right,
		(int)sliderBackground_Master->m_drawSize.top, (int)sliderBackground_Master->m_drawSize.bottom,
		(int)(sliderBackground_Master->m_drawSize.left + sliderBackground_Master->m_drawSize.right) / 2,
		(int)(sliderBackground_Master->m_drawSize.top, sliderBackground_Master->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(MASTER));
	Slider* SFXVolumeSlider = new Slider((int)sliderBackground_SFX->m_drawSize.left, (int)sliderBackground_SFX->m_drawSize.right,
		(int)sliderBackground_SFX->m_drawSize.top, (int)sliderBackground_SFX->m_drawSize.bottom,
		(int)(sliderBackground_SFX->m_drawSize.left + sliderBackground_SFX->m_drawSize.right) / 2,
		(int)(sliderBackground_SFX->m_drawSize.top, sliderBackground_SFX->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(SFX));
	Slider* BGMVolumeSlider = new Slider((int)sliderBackground_BGM->m_drawSize.left, (int)sliderBackground_BGM->m_drawSize.right,
		(int)sliderBackground_BGM->m_drawSize.top, (int)sliderBackground_BGM->m_drawSize.bottom,
		(int)(sliderBackground_BGM->m_drawSize.left + sliderBackground_BGM->m_drawSize.right) / 2,
		(int)(sliderBackground_BGM->m_drawSize.top, sliderBackground_BGM->m_drawSize.bottom) / 2,
		SoundManager::GetInstance().GetChannelGroupVolume(BGM));

	// 슬라이더 이미지 저장
	masterVolumeSlider->SetBackground(sliderBackground_Master);
	masterVolumeSlider->SetFillArea(sliderFillArea_Master);
	masterVolumeSlider->SetHandle(sliderBar_Master);

	BGMVolumeSlider->SetBackground(sliderBackground_BGM);
	BGMVolumeSlider->SetFillArea(sliderFillArea_BGM);
	BGMVolumeSlider->SetHandle(sliderBar_BGM);

	SFXVolumeSlider->SetBackground(sliderBackground_SFX);
	SFXVolumeSlider->SetFillArea(sliderFillArea_SFX);
	SFXVolumeSlider->SetHandle(sliderBar_SFX);

	MasterVolumeSliderObject->SetElement(masterVolumeSlider, ElementType::Slider);
	BGMVolumeSliderObject->SetElement(BGMVolumeSlider, ElementType::Slider);
	SFXVolumeSliderObject->SetElement(SFXVolumeSlider, ElementType::Slider);

	// 슬라이더 오브젝트만 가져서 수치 반영만 한다.
	SoundOptionUI* soundOptionUI = new SoundOptionUI(masterVolumeSlider, BGMVolumeSlider, SFXVolumeSlider);
	SoundOptionUIObject->SetElement(soundOptionUI, ElementType::Script);
	soundOptionUI->SetUISprite(soundOptionUIBackground);
	soundOptionUI->SetMasterTiltle(MasterImage);
	soundOptionUI->SetBgmTiltle(BGImage);
	soundOptionUI->SetSfxTiltle(EffectImage);

	soundOptionUI->Init();

	/// 사운드 옵션 끄기 버튼
	GameObject* soundExitButtonObject = m_gameEngine->CreateObject("soundExitbutton", thisScene);
	Transform* soundExitbuttonTransform = new Transform();
	soundExitButtonObject->SetElement(soundExitbuttonTransform, ElementType::Transform);
	soundExitbuttonTransform->SetLocalPosition({ 597, 0 });
	soundExitbuttonTransform->SetLayer(511);
	soundExitButtonObject->SetActive(false);
	soundExitButtonObject->SetIsDraw(false);

	Button* soundExitButton = new Button(1240, 325, 1275, 360);
	Graphics::Sprite* soundExiteButtonImage = m_directY->ReadSpriteTxtFile("exit_button", m_gameEngine->GetImage("exit_button"));
	soundExitButton->m_sprite = soundExiteButtonImage;
	soundExitButton->SetOnClick([=]()
		{
			if (SoundOptionUIObject->GetActive())
			{
				MainOptionUIObject->SetActive(true);

				soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
				exitButtonObject->SetActive(!exitButtonObject->GetActive());
				SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetActive());
				MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
				SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
				BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
			}
		});
	soundExitButtonObject->SetElement(soundExitButton, ElementType::Button);

	AudioSource* soundExitButtonAudiosource = new AudioSource(m_gameEngine);
	soundExitButtonObject->SetElement(soundExitButtonAudiosource, ElementType::AudioSource);
	soundExitButton->SetAudioSource(soundExitButtonAudiosource);
	soundExitButtonAudiosource->AddSoundVector("sfx_OnButton");
	soundExitButtonAudiosource->AddSoundVector("sfx_StageSelect_click");

	/// 음량 옵션 버튼
	GameObject* ButtonObject = m_gameEngine->CreateObject("soundbutton", thisScene);
	ButtonObject->SetActive(false);

	Transform* buttonTransform = new Transform();
	ButtonObject->SetElement(buttonTransform, ElementType::Transform);
	buttonTransform->SetParent(MainOptionUITransform);
	buttonTransform->SetLocalPosition({ 185, 310 });
	buttonTransform->SetLayer(501);

	Button* soundOptionButton = new Button(830, 635, 1095, 705);
	//Graphics::Sprite* buttonImage = m_directY->ReadSpriteTxtFile("soundbutton", m_gameEngine->GetImage("soundbutton"));
	//soundOptionButton->m_sprite = buttonImage;
	soundOptionButton->SetOnClick([=]()
		{
			if (MainOptionUIObject->GetActive())
			{
				MainOptionUIObject->SetActive(false);

				exitButtonObject->SetActive(!exitButtonObject->GetActive());
				soundExitButtonObject->SetActive(!soundExitButtonObject->GetActive());
				SoundOptionUIObject->SetIsDraw(!SoundOptionUIObject->GetIsDraw());
				MasterVolumeSliderObject->SetActive(!MasterVolumeSliderObject->GetActive());
				SFXVolumeSliderObject->SetActive(!SFXVolumeSliderObject->GetActive());
				BGMVolumeSliderObject->SetActive(!BGMVolumeSliderObject->GetActive());
			}
		});
	ButtonObject->SetElement(soundOptionButton, ElementType::Button);

	AudioSource* soundOptionButtonAudioSource = new AudioSource(m_gameEngine);
	ButtonObject->SetElement(soundOptionButtonAudioSource, ElementType::AudioSource);
	soundOptionButton->SetAudioSource(soundOptionButtonAudioSource);
	soundOptionButtonAudioSource->AddSoundVector("sfx_OnButton");
	soundOptionButtonAudioSource->AddSoundVector("sfx_StageSelect_click");

#pragma endregion SoundOptionUI

#pragma endregion OnClick
	MainOptionButton->SetOnClick([=]()
		{
			MainOptionUIObject->SetActive(true);
			exitButtonObject->SetActive(true);

			mainButtonObject->SetActive(true);
			replayButtonObject->SetActive(true);
			exitButtonObject->SetActive(true);
			ButtonObject->SetActive(true);
		});
	exitButton->SetOnClick([=]()
		{
			MainOptionUIObject->SetActive(false);
			exitButtonObject->SetActive(false);

			mainButtonObject->SetActive(false);
			replayButtonObject->SetActive(false);
			exitButtonObject->SetActive(false);
			ButtonObject->SetActive(false);
		});
#pragma endregion OnClick

#pragma endregion UI

#pragma region Player
	GameObject* PlayerObject = m_gameEngine->CreateObject("Player", thisScene);

	Transform* playerTransform = new Transform();
	PlayerObject->SetElement(playerTransform, ElementType::Transform);
	playerTransform->SetUseCamera(true);
	playerTransform->SetLayer(5);
	playerTransform->SetLocalPosition({ 202.f, 1800.f });

	Player* player = new Player();
	PlayerObject->SetElement(player, ElementType::Script);
	player->SetMyTransform(playerTransform);

	Graphics::Motion* truckMotion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("isekaiTruck", m_gameEngine->GetImage("Truck"), truckMotion);
	player->SetMotion(truckMotion);

	player->SetMyPaticle(m_directY->ReadSpriteTxtFile("starB", m_gameEngine->GetImage("starB")));
	player->SetMyPaticle2(m_directY->ReadSpriteTxtFile("starM", m_gameEngine->GetImage("starM")));

	for (int i = 0; i < 32; i++)
	{
		truckMotion->GetSprite(i)->m_drawPos.x -= truckMotion->GetSprite(i)->m_pivot.x;
		truckMotion->GetSprite(i)->m_drawPos.y -= truckMotion->GetSprite(i)->m_pivot.y;
	}

	Collider* playerCollider = m_gameEngine->GetCollisionManager()->CreateVehicleCollider(0.f, 0.f, 100, 200.f);
	m_gameEngine->GetCollisionManager()->SetPlayerCollider(playerCollider);
	m_gameEngine->GetCollisionManager()->SetPlayerObject(PlayerObject);
	PlayerObject->SetElement(playerCollider, ElementType::Collider);

	AudioSource* playerAudioScource = new AudioSource(m_gameEngine);
	PlayerObject->SetElement(playerAudioScource, ElementType::AudioSource);
	player->SetAudioSource(playerAudioScource);
	playerAudioScource->AddSound("sfx_hit_car_1");
	playerAudioScource->AddSound("sfx_hit_car_1");
	playerAudioScource->AddSound("sfx_hit_npc_1");
	playerAudioScource->AddSound("sfx_hit_npc_1");
	playerAudioScource->AddSound("sfx_hit_wall_1");
	playerAudioScource->AddSound("sfx_truck_start");
	playerAudioScource->AddSound("sfx_engineSoundMId_loop");
	playerAudioScource->AddSound("sfx_engineSoundBase_loop");
	playerAudioScource->AddSound("sfx_trucBack_loop");

#pragma endregion Player

#pragma region GroundRenderManager
	GameObject* GroundRenderManagerObject = m_gameEngine->CreateObject("GroundRenderManager", thisScene);
	Transform* GroundRenderManagerTransform = new Transform();
	GroundRenderManagerObject->SetElement(GroundRenderManagerTransform, ElementType::Transform);
	GroundRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	GroundRenderManagerTransform->SetLayer(0);
	TileRenderManager* groundRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY, TileRenderState::Ground);
	GroundRenderManagerObject->SetElement(groundRenderManager, ElementType::Script);
	groundRenderManager->Init();
	groundRenderManager->ReadSource("Resource/Text/Map/Secret_tile.txt");

#pragma endregion GroundRenderManager

#pragma region TileRenderManager
	GameObject* TileRenderManagerObject = m_gameEngine->CreateObject("TileRenderManager", thisScene);
	Transform* TileRenderManagerTransform = new Transform();
	TileRenderManagerObject->SetElement(TileRenderManagerTransform, ElementType::Transform);
	TileRenderManagerTransform->SetUseCamera(true);
	// 플레이어보다 나중에 그려져야 한다.
	TileRenderManagerTransform->SetLayer(6);
	TileRenderManager* tileRenderManager = new TileRenderManager(m_gameEngine, thisScene, m_directY, TileRenderState::Object);
	TileRenderManagerObject->SetElement(tileRenderManager, ElementType::Script);
	tileRenderManager->Init();
	tileRenderManager->ReadSource("Resource/Text/Map/Secret_object.txt");
#pragma endregion TileRenderManager

#pragma region TileManager
	GameObject* TileManagerObject = m_gameEngine->CreateObject("TileManager", thisScene);
	Transform* TileManagerTransform = new Transform();
	TileManagerObject->SetElement(TileManagerTransform, ElementType::Transform);
	TileManagerTransform->SetUseCamera(true);
	TileManagerTransform->SetLayer(2);
	// 개별 타일의 오브젝트 및 콜라이더를 tileManager Script 안에서 생성함
	TileManager* tileManager = new TileManager(m_gameEngine, m_directY);
	TileManagerObject->SetElement(tileManager, ElementType::Script);
	tileManager->ReadSource2("Resource/Text/Map/Secret_check.txt");
	tileManager->Init();
#pragma endregion TileManager

#pragma region NpcManager
	GameObject* NpcManagerObject = m_gameEngine->CreateObject("NpcManager", thisScene);
	Transform* NpcManagerTransform = new Transform();
	NpcManagerObject->SetElement(NpcManagerTransform, ElementType::Transform);
	NpcManagerTransform->SetUseCamera(true);
	NpcManager* npcManager = new NpcManager(m_gameEngine, thisScene, m_directY);
	NpcManagerObject->SetElement(npcManager, ElementType::Script);
	npcManager->Init();
	npcManager->ReadSource("Resource/Text/Map/Secret_npcLayer.txt");
	npcManager->SetPrintLog([=](std::wstring log, bool isNpc) { scrollbar->AddText(log, isNpc); });
	//npcManager->MakeNpc();
	npcManager->MakeObject();
#pragma endregion NpcManager

#pragma region CarNpcManager
	GameObject* CarNpcManagerObject = m_gameEngine->CreateObject("CarNpcManager", thisScene);
	Transform* CarNpcManagerTransform = new Transform();
	CarNpcManagerObject->SetElement(CarNpcManagerTransform, ElementType::Transform);
	CarNpcManagerTransform->SetUseCamera(true);
	NpcManager* CarnpcManager = new NpcManager(m_gameEngine, thisScene, m_directY, NpcType::Car);
	CarNpcManagerObject->SetElement(CarnpcManager, ElementType::Script);
	CarnpcManager->Init();
	CarnpcManager->ReadSource("Resource/Text/Map/Secret_carLayer.txt");
	CarnpcManager->MakeObject();
#pragma endregion CarNpcManager

#pragma region NpcWaitManager
	///NPC대기열을 관리하는 오브젝트

	GameObject* npcWaitManagerObject = m_gameEngine->CreateObject("NpcWaitManager", thisScene);
	Transform* npcWaitManagerTransform = new Transform;
	npcWaitManagerObject->SetElement(npcWaitManagerTransform, ElementType::Transform);

	NPCWaitManager* npcWaitManager = new NPCWaitManager(npcManager);
	npcWaitManagerObject->SetElement(npcWaitManager, ElementType::Script);

	///면접자를 알아야한다
	heroFaceUI->SetInterviewList(npcWaitManager->GetWaitQueue());
#pragma endregion NpcWaitManager

#pragma region MessageManger
	GameObject* messageManagerObject = m_gameEngine->CreateObject("messageManager", thisScene);
	Transform* messageManagerTransform = new Transform();
	messageManagerObject->SetElement(messageManagerTransform, ElementType::Transform);
	messageManagerTransform->SetUseCamera(true);
	messageManagerTransform->SetLayer(500);

	MessageManager* messageManager = new MessageManager(npcWaitManager, nullptr);
	messageManagerObject->SetElement(messageManager, ElementType::Script);
	messageManager->ReadMessageCSV("Resource/NPC_message_list");
	messageManager->ReadDialogCSV("Resource/NPC_message");
	messageManager->SetQueue(npcWaitManager->GetWaitQueue());
	messageManager->SetClearCount(2);
#pragma endregion MessageManger
}