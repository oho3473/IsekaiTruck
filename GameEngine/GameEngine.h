#pragma once
#include <Windows.h>
#include <string>
#include <functional>

class TimeManager;
//class InputManager;
class ResourceManager;
//class SoundManager;
//class CameraManager;
class SceneManager;
class CollisionManager;

class DirectY;
struct ID2D1Bitmap;

class GameObject;
class Scene;

class GameProcess;

namespace FMOD { class Sound; }

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void Init(const HWND hWnd, DirectY* directY);

	void FixedUpdate();
	void Update();
	void LateUpdate();

	void PreRender();
	void Render(DirectY* directY);
	void PostRender(DirectY* directY);

	void Release();

	// Scene
	void CreateScene(std::string sceneName, std::function<void()> initFunction);
	void SetNextScene(std::string sceneName);
	void LoadScene(std::string sceneName);
	Scene* FindScene(std::string sceneName);
	Scene* GetPreScene();
	Scene* GetCurrentScene();
	GameObject* CreateObject(const std::string& objectName, Scene* scene);

	// Resource
	ID2D1Bitmap* GetImage(std::string imageName);
	FMOD::Sound* GetSound(std::string soundName);
	void LoadResource(std::string sceneName, DirectY* directY);

	// Time
	int GetFPS();
	float GetDeltaTime();
	void SetTimeScale(float timeScale);
	
	// Collider
	CollisionManager* GetCollisionManager() const { return m_collisionManager; }

private:
	TimeManager*		m_timeManager;
	//InputManager*		m_inputManager; singleton
	ResourceManager*	m_resourceManager;
	//SoundManager*		m_soundManager;	singleton
	//CameraManager*	m_cameraManager; singleton
	SceneManager*		m_sceneManager;
	CollisionManager*	m_collisionManager;
};
