#include "SceneManager.h"
#include "ResourceManager.h"

#include "Scene.h"

#include "../DirectY/DirectY.h"

#include "SoundManager.h"

SceneManager::SceneManager()
	: m_currentScene(nullptr), m_nextScene(nullptr), m_scenes(),
	m_resourceManager(nullptr), m_preScene(nullptr)
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Init(ResourceManager* resourceManager)
{
	// 리소스를 불러오기 위해 멤버 변수를 가지고 있는다. 고민중...
	m_resourceManager = resourceManager;
}

void SceneManager::FixedUpdate()
{
	m_currentScene->FixedUpdate();
}

void SceneManager::Update(float deltaTime)
{
	m_currentScene->Update(deltaTime);
}

void SceneManager::LateUpdate()
{
	//m_currentScene->LateUpdate();
	if (m_nextScene != nullptr)
	{
		LoadScene(m_nextScene->GetName());
	}
}

void SceneManager::PreRender()
{
	m_currentScene->PreRender();
}

void SceneManager::Render(DirectY* directY)
{
	m_currentScene->Render(directY);
}

void SceneManager::PostRender(DirectY* directY)
{
	m_currentScene->PostRender(directY);
}

void SceneManager::Release()
{
	// ResourceManager와 directY엔진은 GameProcess에서 삭제한다.
	for (auto& scene : m_scenes)
	{
		delete scene.second;
	}
	m_scenes.clear();
}

void SceneManager::LoadScene(std::string sceneName)
{
	static int loadCount = 0;

	// 첫 로드가 아니면
	if (loadCount > 0)
	{
		// 씬 변경 -> 추가적인 처리가 필요하다. 아직 미완성
		m_currentScene->DeleteAllObject();
		// 이전 씬의 리소스를 메모리에서 해제한다.
		m_resourceManager->Release();
	}
	
	m_preScene = m_currentScene;

	// 현재 씬 변경
/*	SoundManager::GetInstance().StopSound();*/
	m_currentScene = m_scenes[sceneName];
	m_sceneInitFunctions[sceneName]();

	m_currentScene->Init();

	m_nextScene = nullptr;

	loadCount++;
}

void SceneManager::CreateScene(std::string sceneName, std::function<void()> initFunction)
{
	Scene* newScene = new Scene(sceneName);
	m_scenes[sceneName] = newScene;
	m_sceneInitFunctions[sceneName] = initFunction;
}

Scene* SceneManager::FindScene(std::string sceneName)
{
	return m_scenes[sceneName];
}
