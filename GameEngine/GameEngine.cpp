#include "GameEngine.h"

#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "CameraManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "../GameProcess/ClearManager.h"

#include "Scene.h"

#include "GameObject.h"

GameEngine::GameEngine()
{
	m_timeManager = new TimeManager;
	// inputManager is singleton
	m_resourceManager = new ResourceManager;
	// soundManager is singleton
	m_sceneManager = new SceneManager;
	m_collisionManager = new CollisionManager;
}

GameEngine::~GameEngine()
{
	Release();
}

void GameEngine::Init(const HWND hWnd, DirectY* directY)
{
	m_timeManager->Init();
	InputManager::GetInstance().Init(hWnd);
	m_resourceManager->Init();
	SoundManager::GetInstance().Init();
	CameraManager::GetInstance().Init();
	m_sceneManager->Init(m_resourceManager);
	m_collisionManager->Init();
}

void GameEngine::FixedUpdate()
{
	 m_sceneManager->FixedUpdate();
}

void GameEngine::Update()
{
	m_timeManager->Update();
	InputManager::GetInstance().Update();
	SoundManager::GetInstance().Update();
	CameraManager::GetInstance().Update();
	m_collisionManager->Update(m_timeManager->GetDeltaTime());
	m_sceneManager->Update(m_timeManager->GetDeltaTime());
}

void GameEngine::LateUpdate()
{
	m_sceneManager->LateUpdate();
}

void GameEngine::PreRender()
{
	m_sceneManager->PreRender();
}

void GameEngine::Render(DirectY* directY)
{
	m_collisionManager->Render(*directY);
	m_sceneManager->Render(directY);
}

void GameEngine::PostRender(DirectY* directY)
{
	m_sceneManager->PostRender(directY);
}

void GameEngine::Release()
{
	delete m_collisionManager;

	m_sceneManager->Release();
	delete m_sceneManager;

	m_resourceManager->Release();
	delete m_resourceManager;

	delete m_timeManager;
}

void GameEngine::CreateScene(std::string sceneName, std::function<void()> initFunction)
{
	m_sceneManager->CreateScene(sceneName, initFunction);
}

Scene* GameEngine::FindScene(std::string sceneName)
{
	return m_sceneManager->FindScene(sceneName);
}

Scene* GameEngine::GetPreScene()
{
	return m_sceneManager->GetPreScene(); 
}

Scene* GameEngine::GetCurrentScene()
{
	return m_sceneManager->GetCurrentScene();
}

void GameEngine::SetNextScene(std::string sceneName)
{
	//test Сп
	m_collisionManager->Init();

	m_sceneManager->SetNextScene(sceneName);
}

void GameEngine::LoadScene(std::string sceneName)
{
	m_sceneManager->LoadScene(sceneName);
}

GameObject* GameEngine::CreateObject(const std::string& objectName, Scene* scene)
{
	return scene->CreateObject(objectName);
}

ID2D1Bitmap* GameEngine::GetImage(std::string imageName)
{
	return m_resourceManager->GetImage(imageName);
}

FMOD::Sound* GameEngine::GetSound(std::string soundName)
{
	return m_resourceManager->GetSound(soundName);
}

void GameEngine::LoadResource(std::string sceneName, DirectY* directY)
{
	m_resourceManager->LoadResource(sceneName, *directY);
}

int GameEngine::GetFPS()
{
	return m_timeManager->GetFps();
}

float GameEngine::GetDeltaTime()
{
	return m_timeManager->GetDeltaTime();
}

void GameEngine::SetTimeScale(float timeScale)
{
	TimeManager::m_timeScale = timeScale;
}
