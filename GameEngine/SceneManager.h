#pragma once

#include <unordered_map>
#include <string>
#include <functional>

class Scene;

class ResourceManager;
class DirectY;
class GameProcess;

/// <summary>
/// Scene을 관리한다.
/// </summary>
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Init(ResourceManager* resourceManager);

	void FixedUpdate();
	void Update(float deltaTime);
	void LateUpdate();

	void PreRender();
	void Render(DirectY* directY);
	void PostRender(DirectY* directY);

	void Release();

	void SetNextScene(std::string sceneName) { m_nextScene = m_scenes[sceneName]; }
	void LoadScene(std::string sceneName);
	void CreateScene(std::string sceneName, std::function<void()> initFunction);
	Scene* FindScene(std::string sceneName);

	Scene*	GetCurrentScene() { return m_currentScene; }
	Scene*	GetPreScene() { return m_preScene; }
	void	SetCurrentScene(std::string sceneName) { m_currentScene = m_scenes[sceneName]; }

private:
	Scene* m_currentScene;
	Scene* m_nextScene;
	Scene* m_preScene;
	std::unordered_map<std::string, Scene*> m_scenes;
	std::unordered_map<std::string, std::function<void()>> m_sceneInitFunctions;

	ResourceManager* m_resourceManager;
};

