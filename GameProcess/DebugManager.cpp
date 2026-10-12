#include "DebugManager.h"
#include "InputManager.h"
#include "GameEngine.h"
#include "DirectY.h"
#include "GameObject.h"

void DebugManager::Update(float deltaTime)
{
	if (InputManager::GetInstance().IsGetKeyDown(VK_F1))
	{
		m_isDebuging = !m_isDebuging;
	}

	if (InputManager::GetInstance().IsGetKeyDown(VK_F2))
	{
		m_tileDebuging = !m_tileDebuging;
	}

	if (InputManager::GetInstance().IsGetKeyDown('1'))
	{
		m_directY->DeletePaticleEffect();
		m_gameEngine->SetNextScene("GameClearScene");
	}

	if (InputManager::GetInstance().IsGetKeyDown('2'))
	{
		m_directY->DeletePaticleEffect();
		m_gameEngine->SetNextScene("GameOverScene");
	}

	if (InputManager::GetInstance().IsGetKeyDown('3'))
	{
		if (m_object != nullptr)
		{
			m_object->SetActive(true);
			m_object = nullptr;
			m_gameEngine->SetTimeScale(0);
		}
		// 타이머도 꺼야될 거 같긴 한데 그냥 하자
	}

// 	if (InputManager::GetInstance().IsGetKeyDown('4'))
// 	{
// 		m_isDebuging = !m_isDebuging;
// 	}
// 
// 	if (InputManager::GetInstance().IsGetKeyDown('5'))
// 	{
// 		m_isDebuging = !m_isDebuging;
// 	}
// 
// 	if (InputManager::GetInstance().IsGetKeyDown('6'))
// 	{
// 		m_isDebuging = !m_isDebuging;
// 	}
// 
// 	if (InputManager::GetInstance().IsGetKeyDown('7'))
// 	{
// 		m_isDebuging = !m_isDebuging;
// 	}
}