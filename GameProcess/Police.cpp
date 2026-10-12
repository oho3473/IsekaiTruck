#include "Police.h"
#include "PoliceState.h"
#include "FSM.h"

#include "AudioSource.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "Player.h"
#include "DebugManager.h"
#include "InputManager.h"

#include "../MathEngine/PathFinding.h"
#include "PathFindRenderer.h"

Police::Police(Transform* playerTransform, Transform* policeTrasform, const MathEngine::Vector2 policeStation, std::string mapDataPath, std::vector<MathEngine::Vector2> patrolPath)
	: m_curState()
	, m_FSM()
	, m_playerTransform(playerTransform)
	, m_policeTrasnform(policeTrasform)
	, m_police()
	, m_redPolice()
	, m_bluePolice()
	, m_pathManager(nullptr)
	, m_index(0)
	, m_time(0)
	, m_spriteIndex(4)
	, m_preDirection(-1.f, -1.f)
	, m_direction(1.f, 1.f)
	, m_policeStation(policeStation)
	, m_mapDataPath(mapDataPath)
	, m_audioSource(nullptr)
	, m_isInterrogated(false)
	, m_interrogatedTime(0.f)
	, m_recognizeRange(MathEngine::Vector2(400.f, 400.f))
	, m_route(patrolPath)
	, m_pathfind(new Algorithm::PathFinding())
	, m_speed(200.f)
{
	Init();
}

Police::~Police()
{
	delete m_FSM;
	delete m_police;
	delete m_redPolice;
	delete m_bluePolice;
	delete m_pathfind;
}

void Police::Init()
{
	m_curState = State::Idle;
	m_FSM = new FSM(new PoliceIdleState(this));
	m_pathfind->ReadSource(m_mapDataPath);
}

void Police::FixedUpdate()
{

}

void Police::Update(float deltaTime)
{
	static float indexTime = 0;
	indexTime += deltaTime;

	if (indexTime > 0.5f)
	{
		m_index++;
		indexTime = 0;
	}

	switch (m_curState)
	{
		case Police::State::Idle:
			m_audioSource->StopSound("sfx_arrest");
			if (IsRecognizePlayer())
			{
				if (!(m_audioSource->isPlaying("sfx_siren")))
				{
					m_audioSource->PlayBack("sfx_siren");
				}
				ChangeState(State::Chasing);
			}
			break;
		case Police::State::Chasing:

			if (!IsRecognizePlayer())
			{
				ChangeState(State::Idle);
			}
			if (IsCollision())
			{
				ChangeState(State::Collision);
			}
			break;
		case Police::State::Collision:
			// 체포됨 
			if (!(m_audioSource->isPlaying("sfx_arrest")))
			{
				m_audioSource->PlayBack("sfx_arrest", 0.4f, FMOD_LOOP_NORMAL);
			}
			break;
	}
	m_time += deltaTime;

	if (m_time > 0.005f)
	{
		m_time = 0;

		/// 치트 키
		float addSpeed = 0.5f;
		if (InputManager::GetInstance().IsGetKeyDown('O'))
		{
			m_speed += addSpeed;
		}

		if (InputManager::GetInstance().IsGetKeyDown('P') && m_speed > 0.f)
		{
			m_speed -= addSpeed;

			if (m_speed < 0.f)
			{
				m_speed = 0.f;
			}
		}

		/// FSM
		m_FSM->UpdateState(deltaTime);

		/// 스프라이트 인덱스 관련
		// 인덱스 변경
		if ((m_preDirection.x * m_direction.x < 0.f) || (m_preDirection.y * m_direction.y < 0.f))
		{
			float crossProduct = m_preDirection.Cross(m_direction);
			// 우회전
			if (crossProduct > 0.f)
			{
				m_spriteIndex++;
			}
			// 좌회전
			else if (crossProduct < 0.f)
			{
				m_spriteIndex--;
			}
		}
	}

	if (m_index > 32)
	{
		m_index = 0;
	}

	// 인덱스 보정
	if (m_spriteIndex < 0)
	{
		m_spriteIndex += 31;
	}

	m_spriteIndex %= 32;

	// 이전 방향 업뎃
	if (m_direction.x > 0.f && m_direction.y > 0.f && m_spriteIndex == 4 ||
		m_direction.x < 0.f && m_direction.y > 0.f && m_spriteIndex == 12 ||
		m_direction.x < 0.f && m_direction.y < 0.f && m_spriteIndex == 20 ||
		m_direction.x > 0.f && m_direction.y < 0.f && m_spriteIndex == 28)
	{
		m_preDirection = m_direction;
	}

	// 심문 한다면
	if (m_isInterrogated)
	{
		m_interrogatedTime += deltaTime;

		if ((m_interrogatedTime >= 5.f))
		{
			GameObject* playerObj = m_playerTransform->GetGameObject();
			playerObj->SetActive(true);
			m_isInterrogated = true;
			m_interrogatedTime = 0.f;
			m_recognizeRange = { 400.f, 400.f };
		}
	}
}

void Police::LateUpdate()
{

}

void Police::PreRender()
{

}

void Police::Render(DirectY* directY)
{
	
	switch (m_curState)
	{
		case Police::State::Idle:
			directY->DrawBitmap(m_police->GetSprite(m_spriteIndex));
			if (DebugManager::GetInstance().GetIsDebuging())
			{
				directY->DrawText(0, 40, 20, TextAlignMent::LEFT, L"Verdana", "%s", "Idle");
			}
			break;
		case Police::State::Chasing:
		{
			if (m_index % 2 == 0)
			{
				directY->DrawBitmap(m_bluePolice->GetSprite(m_spriteIndex));
			}
			else if (m_index % 2 == 1)
			{
				directY->DrawBitmap(m_redPolice->GetSprite(m_spriteIndex));
			}
		}
		if (DebugManager::GetInstance().GetIsDebuging())
		{
			directY->DrawText(0, 40, 20, TextAlignMent::LEFT, L"Verdana", "%s", "Chasing");
		}
		break;
		case Police::State::Collision:
			directY->DrawBitmap(m_police->GetSprite(m_spriteIndex));
			//directY->DrawText(0, 40, 20, TextAlignMent::LEFT, L"Verdana", "%s", "Collision");
			break;
	}

	//directY->DrawBitmap(m_police->GetSprite(m_spriteIndex));

	m_FSM->RenderState(directY);
}

void Police::PostRender(DirectY* directY)
{

}

void Police::Release()
{

}

ElementType Police::GetElementType() const
{
	return ElementType::Script;
}

GameObject* Police::GetGameObject() const
{
	return m_gameObject;
}

void Police::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void Police::ChangeState(State nextState)
{
	m_curState = nextState;
	switch (m_curState)
	{
		case Police::State::Idle:
			m_FSM->ChangeState(new PoliceIdleState(this));
			break;
		case Police::State::Chasing:
			m_FSM->ChangeState(new PoliceChasingState(this));
			break;
		case Police::State::Collision:
			m_FSM->ChangeState(new PoliceCollisionState(this));
			break;
		default:
			break;
	}
}

bool Police::IsRecognizePlayer()
{
	// 일단은 거리로 대충 해놓기
	int playerX = std::abs((int)m_playerTransform->GetScreenPosition().x);
	int playerY = std::abs((int)m_playerTransform->GetScreenPosition().y);

	int policeX = std::abs((int)m_policeTrasnform->GetScreenPosition().x);
	int policeY = std::abs((int)m_policeTrasnform->GetScreenPosition().y);

	int maxX = max(std::abs((int)m_playerTransform->GetScreenPosition().x), std::abs((int)m_policeTrasnform->GetScreenPosition().x));
	int minX = min(std::abs((int)m_playerTransform->GetScreenPosition().x), std::abs((int)m_policeTrasnform->GetScreenPosition().x));

	int maxY = max(std::abs((int)m_playerTransform->GetScreenPosition().y), std::abs((int)m_policeTrasnform->GetScreenPosition().y));
	int minY = min(std::abs((int)m_playerTransform->GetScreenPosition().y), std::abs((int)m_policeTrasnform->GetScreenPosition().y));

	// 자기 포지션도 가지고 있어야 함
	if (maxX - minX < (int)m_recognizeRange.x && maxY - minY < (int)m_recognizeRange.y)
	{
		return true;
	}
	return false;
}

bool Police::IsCollision()
{
	Collider* myCollider = ((Collider*)(m_gameObject->GetElement(ElementType::Collider)));
	return myCollider->GetIsCollision();
}