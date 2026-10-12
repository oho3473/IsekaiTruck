
#include "WalkingNPC.h"
#include "FSM.h"
#include "WalkingNPCState.h"


#include "../MathEngine/PathFinding.h"

#include "Transform.h"
WalkingNPC::WalkingNPC(Transform* npcTransform)
	:m_pathFind(nullptr)
	, m_currentDestination()
	, m_destinations{}
	, m_destinationCount(0)
	, m_destinationIndex(0)
	, m_transform(npcTransform)
{
	Init();

}

WalkingNPC::~WalkingNPC()
{
	delete m_FSM;
	delete m_motion;
	delete m_pathFind;
}

void WalkingNPC::Init()
{
	m_pathFind = new Algorithm::PathFinding();
	m_FSM = new FSM(new WalkingNPCIdleState(this));
	
	// Idle일 때 첫 목적지 설정
	m_curState = State::Idle;
	// Idle 실행
	ChangeState(State::Idle);

	// 맵 읽기
	m_pathFind->ReadSource("Text/Map/Stage1_check.txt");


	// 시작 위치와 목적지 재설정
	//m_pathFind->SetSatrtNode(m_transform->GetLocalPosition());
	//m_pathFind->SetEndNode(m_destinations[0]);

	// 다음 경로로 이동
	// m_pathFind->nearestLocation();
	// 경로 탐색
	//m_pathFind->SolveAstar();

	// 최종 경로 업데이트
	//m_pathFind->nearestLocation();
	//m_pathFind->GetFinalPath();

}

void WalkingNPC::FixedUpdate()
{

}

void WalkingNPC::Update(float deltaTime)
{
	switch (m_curState)
	{
	case WalkingNPC::State::Idle:
		// 새로운 길을 찾고 Walk로 변경!
		// 이렇게 하는게 맞는지는 모르겠지만 WalkingNPCState에서 SetNextDestination으로 바꾸기는 함.
		// 아니 안함
		SetNextDestination();
		PathFind();
		ChangeState(State::Walk);
		break;

	case WalkingNPC::State::Walk:
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
			break;
		}

		// 아무것도 아닐 경우 계속 이동
		// Move();

		break;
	case WalkingNPC::State::Attacked:
		// 차에 부딫히면 오른쪽 UI로 날려보내기!
		
		break;
	default:
		break;
	}
	m_FSM->UpdateState();
}

void WalkingNPC::LateUpdate()
{
}

void WalkingNPC::PreRender()
{
}

void WalkingNPC::Render(DirectY* directY)
{
	directY->DrawBitmap(m_motion->GetSprite(0));
}

void WalkingNPC::PostRender(DirectY* directY)
{
}

void WalkingNPC::Release()
{
}

ElementType WalkingNPC::GetElementType() const
{
	return ElementType::Script;
}

GameObject* WalkingNPC::GetGameObject() const
{
	return m_gameObject;
}

void WalkingNPC::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}


void WalkingNPC::SetNextDestination()
{
// 	m_currentDestination = m_destinations[m_destinationIndex];
// 	m_destinationIndex++;
// 	m_destinationIndex %= m_destinationCount;
}

void WalkingNPC::PathFind()
{
	return;
	// path find start

	m_currentDestination;	// 목적지

	m_pathFind->SetSatrtNode(m_transform->GetLocalPosition());
	m_pathFind->SetEndNode(m_destinations[0]);

}

bool WalkingNPC::IsArrive()
{
	// 도착 했는지 여부 확인
	return false;
}

bool WalkingNPC::IsAttacked()
{
	// 차에 치였는지 여부 확인
	return false;
}

void WalkingNPC::Walk()
{
	//m_pathFind->nearestLocation();
}

void WalkingNPC::ChangeState(State nextState)
{
	m_curState = nextState;
	switch (m_curState)
	{
	case WalkingNPC::State::Idle:
		m_FSM->ChangeState(new WalkingNPCIdleState(this));
		break;
	case WalkingNPC::State::Walk:
		m_FSM->ChangeState(new WalkingNPCWalkState(this));
		break;
	case WalkingNPC::State::Attacked:
		m_FSM->ChangeState(new WalkingNPCAttackedState(this));
		break;
	default:
		break;
	}
}
