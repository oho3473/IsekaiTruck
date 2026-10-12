#pragma once
#include "Singleton.h"

class GameEngine;
class DirectY;
class GameObject;

class DebugManager : public Singleton<DebugManager>
{
public:
	void Update(float deltaTime);

	bool GetIsDebuging() { return m_isDebuging; }
	bool GetTileDebuging() { return m_tileDebuging; }
	void SetGameEngine(GameEngine* gameEngine) { m_gameEngine = gameEngine; }
	void SetDirectY(DirectY* directY) { m_directY = directY; }
	void SetObject(GameObject* gameObject) { m_object = gameObject; }
private:
	bool m_isDebuging;
	bool m_tileDebuging;

	GameEngine* m_gameEngine;
	DirectY* m_directY;
	GameObject* m_object;
	
	// 한 개의 클래스라는 것을 보장하려면 생성자를 숨겨야 한다.
	DebugManager() : m_isDebuging(false), m_gameEngine(nullptr), m_directY(nullptr), m_object(nullptr) {}
	~DebugManager() {}

	friend class Singleton<DebugManager>;
};

