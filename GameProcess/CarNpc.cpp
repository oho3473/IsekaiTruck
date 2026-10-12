#include "CarNpc.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "Animator.h"
#include "FSM.h"
//#include "WalkingNPCState.h"
#include "CarNPCState.h"

#include "../MathEngine/PathFinding.h"
#include "Vector2.h"
#include <math.h>
#include "MotionAnimator.h"
#include "FindPosition.h"

CarNpc::CarNpc(Transform* transform)
	:m_transform(transform)
	, m_preDirection({ -1.f,-1.f })
	, m_direction({0.f, 0.f})
	, m_spriteIndex(0)
	, m_tileLayer(0)
	, m_size(60)
	, m_currentLayer(0)
{
	m_FSM = new FSM(new CarNPCIdleState(this));
	m_findPosition = new Algorithm::FindPosition();
	// 첫 경로설정
	m_pathIndex = 0;
	// Idle일 때 첫 목적지 설정
	m_curState = State::Idle;
	// Idle 실행
	ChangeState(State::Idle);


	// 레이어 초기화
	m_findPosition->Init();
	MathEngine::Vector2 renderPos(0, 0);
	int prevY = 0;
	int prevX = 0;
	while (true)
	{
		int y = static_cast<int>(renderPos.y);
		int x = static_cast<int>(renderPos.x);

		if (renderPos.x == 0)
		{
			// 타일 레이어 증가
			m_tileLayer++;
		}

		if (renderPos.y == m_size - 1)
		{
			m_tileLayer++;
		}
		m_layerArr[y][x] = m_tileLayer;

		if (renderPos.x == m_size - 1 && renderPos.y == m_size - 1)
		{
			break;
		}

		if (renderPos.x == m_size - 1)
		{
			prevX++;
			renderPos.x = static_cast<float>(prevX);
			renderPos.y = static_cast<float>(prevY);
		}
		else if (renderPos.y == 0)
		{
			prevY++;
			renderPos.y = static_cast<float>(prevY);
			renderPos.x = 0;
		}
		else
		{
			renderPos.y -= 1;
			renderPos.x += 1;
		}

		if (renderPos.y > m_size - 1)
			renderPos.y--;
		if (renderPos.x > m_size - 1)
			renderPos.x--;
	}
}

CarNpc::~CarNpc()
{
	delete m_FSM;
	delete m_motion;
	delete m_findPosition;
}

void CarNpc::Init()
{
	//m_animator->SetCurrentMotion("_back_right_walking");
}

void CarNpc::FixedUpdate()
{

}

void CarNpc::Update(float deltaTime)
{
	switch (m_curState)
	{
	case CarNpc::State::Idle:
		// 새로운 길을 찾고 Walk로 변경!
		// 이렇게 하는게 맞는지는 모르겠지만 WalkingNPCState에서 SetNextDestination으로 바꾸기는 함.
		// 아니 안함
		SetNextDestination();
		//PathFind();
		ChangeState(State::Move);
		break;

	case CarNpc::State::Move:
		// 차에 부딪혔을 경우 
		if (IsAttacked())
		{
			ChangeState(State::Attacked);
			break;
		}

		// 도착했을 경우
		if (IsArrive())
		{
			ChangeState(State::Idle);
			//SetNextDestinationTestVer();
			break;
		}

		// 아무것도 아닐 경우 계속 이동
		Move(deltaTime);
		m_gameObject;
		break;
	case CarNpc::State::Attacked:
		// 차에 부딫히면 오른쪽 UI로 날려보내기!
		break;
	default:
		break;
	}
	m_FSM->UpdateState(deltaTime);


	/// 스프라이트 인덱스 관련

	m_time += deltaTime;

	if (m_time > 0.03f)
	{
		m_time = 0;

		// 인덱스 변경
		if ((m_preDirection.x * m_direction.x < 0.f) || (m_preDirection.y * m_direction.y < 0.f))
		{
			float crossProduct = m_preDirection.Cross(m_direction);
			// 우회전
			if (crossProduct >= 0.f)
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

	// 인덱스 보정
	if (m_spriteIndex < 0)
	{
		m_spriteIndex += 29;
	}

	m_spriteIndex %= 30;

	// 이전 방향 업뎃
	if (m_direction.x > 0.f && m_direction.y > 0.f && m_spriteIndex == 4 ||
		m_direction.x < 0.f && m_direction.y > 0.f && m_spriteIndex == 11 || 
		m_direction.x < 0.f && m_direction.y < 0.f && m_spriteIndex == 18 ||
		m_direction.x > 0.f && m_direction.y < 0.f && m_spriteIndex == 26)
	{
		m_preDirection = m_direction;
	}
}

void CarNpc::LateUpdate()
{

}

void CarNpc::PreRender()
{
	FindThisPositionAndSetLayer();
}

void CarNpc::Render(DirectY* directY)
{
	m_directY = directY;
	directY->DrawBitmap(m_motion->GetSprite(m_spriteIndex));
}

void CarNpc::PostRender(DirectY* directY)
{

}

void CarNpc::Release()
{

}

ElementType CarNpc::GetElementType() const
{
	return ElementType::Script;
}

GameObject* CarNpc::GetGameObject() const
{
	return m_gameObject;
}

void CarNpc::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void CarNpc::SetNextDestination()
{
	m_beforePathIndex = m_pathIndex;

	m_transform->SetLocalPosition(m_paths[m_pathIndex]);
	// 다음 경로 설정
	if (!isReverse)
	{
		m_pathIndex++;
	}
	else
	{
		m_pathIndex--;
	}

	if (m_pathIndex >= static_cast<int>(GetPaths().size()))
	{
		isReverse = true;
		//m_pathIndex--;
		m_pathIndex-=2;
	}
	if (m_pathIndex < 0)
	{
		isReverse = false;
		m_pathIndex = 0;
	}
	m_currentDestination = GetPaths()[m_pathIndex];

	MathEngine::Vector2 currentPos(m_transform->GetTransformMatrix().dx, m_transform->GetTransformMatrix().dy);
	// 이동
	m_currentDirection = GetPaths()[m_pathIndex] - currentPos;
	m_currentDirection = m_currentDirection.Normalize();

	// npc의 방향 설정
	SetNextDirection();
}

bool CarNpc::IsArrive()
{
	// TODO : 현재 도착했는지를 검사!
	Vector2 currentPos = m_transform->GetLocalPosition();
	if (m_currentDestination.x + 10.f >= currentPos.x
		&& m_currentDestination.x - 10.f <= currentPos.x
		&& m_currentDestination.y + 10.f >= currentPos.y
		&& m_currentDestination.y - 10.f <= currentPos.y)
	{
		// m_transform->SetLocalPosition(m_currentDestination);
		return true;
	}

	return false;
}

bool CarNpc::IsAttacked()
{
	// TODO : 차에 치였는지를 검사!
	//return isAccident;

	// 이건 차다
	return false;
}

void CarNpc::Move(float deltaTime)
{
	m_transform->Translate(m_currentDirection * m_speed * deltaTime);
}

void CarNpc::MoveInUI(float deltaTime)
{

}

void CarNpc::ChangeState(State nextState)
{
	m_curState = nextState;
	switch (m_curState)
	{
	case CarNpc::State::Idle:
		m_FSM->ChangeState(new CarNPCIdleState(this));
		break;
	case CarNpc::State::Move:
		m_FSM->ChangeState(new CarNPCMoveState(this));
		break;
	case CarNpc::State::Attacked:
		m_FSM->ChangeState(new CarNPCAttackedState(this));
		break;
	default:
		break;
	}
}

void CarNpc::SetNextDirection()
{
	CarVectorInt currentPos = m_pathArr[m_beforePathIndex];
	CarVectorInt nextPos = m_pathArr[m_pathIndex];

	// 이거는 자동차 애니메이션을 만들기 위한 과정 , 그리고 이 아래는 회전하기 전의 위치
	//m_preDirection.x = (float)m_pathArr[m_beforePathIndex].x;
	//m_preDirection.y = (float)m_pathArr[m_beforePathIndex].y;
	//// 여기까지


	//// 이거는 자동차 애니메이션을 만들기 위한 과정 , 그리고 이 아래는 회전한 후의 위치
	//m_direction.x = (float)m_pathArr[m_pathIndex].x;
	//m_direction.y = (float)m_pathArr[m_pathIndex].y;
	// 여기까지


	Vector2 direction = m_paths[m_pathIndex] - m_paths[m_beforePathIndex] ;
	direction.Normalize();
	m_direction = direction;


	//if (currentPos.x == nextPos.x)
	//{
	//	if (currentPos.y <= nextPos.y)
	//	{
	//		m_curDirState = Direction::Down;
	//	}
	//	else
	//	{
	//		m_curDirState = Direction::Up;
	//	}
	//}
	//else if (currentPos.y == nextPos.y)
	//{
	//	if (currentPos.x <= nextPos.x)
	//	{
	//		m_curDirState = Direction::Right;
	//	}
	//	else
	//	{
	//		m_curDirState = Direction::Left;
	//	}
	//}
	//else
	//{
	//	m_curDirState;
	//}
}

void CarNpc::FindThisPositionAndSetLayer()
{
	// 배열 좌표, 보정된 위치
	std::pair<MathEngine::Vector2, MathEngine::Vector2> getData
		= m_findPosition->FindNearest(m_transform->GetLocalPosition());

	m_currentLayer = m_layerArr[getData.first.x][getData.first.y];

	// 여기서 레이어 설정
	m_transform->SetLayer(m_currentLayer + 1);
}
