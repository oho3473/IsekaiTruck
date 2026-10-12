#include "SkinHeadNPC.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "Animator.h"
#include "FSM.h"
#include "WalkingNPCState.h"
#include "FindPosition.h"
#include "../MathEngine/PathFinding.h"
#include "AudioSource.h"

#include <math.h>

SkinHeadNPC::SkinHeadNPC(Transform* npcTransform)
	: m_transform(npcTransform),
	m_motion(nullptr), m_index(0),
	m_sumTime(0), m_degree(0),
	waitNumber(-1),
	m_targetScale(0),
	isAccident(false),
	m_directY(nullptr),
	m_myPos{}, m_topPos{ 1200, 200 }, m_TargetPos{ 1730 - 280, 720 }, m_startPos{}
	, m_curState{}
	, m_FSM(nullptr)
	, m_currentDestination{}
	, m_paths{}
	, m_pathIndex(0)
	, m_destinationCount(0)
	, m_destinationIndex(0)
	, isReverse(false)
	, m_speed(200.f)
	, m_currentDirection{}
	, m_isHero(false)
	, m_time(0)
	, m_audioSource(nullptr)
	, m_tileLayer(0)
	, m_size(60)
	, m_currentLayer(0)
{
	m_FSM = new FSM(new WalkingNPCIdleState(this));
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

SkinHeadNPC::~SkinHeadNPC()
{
	int a = 3;
	//delete m_motion;

	delete m_FSM;
	delete m_findPosition;
}

void SkinHeadNPC::Init()
{
	m_animator->SetCurrentMotion("_front_left_walking");
	m_audioSource = (AudioSource*)(m_gameObject->GetElement(ElementType::AudioSource));

	if (GetCurState() == State::Credit) return;
	m_transform->SetLocalScale({ 0.5f,0.5f });
}

void SkinHeadNPC::FixedUpdate()
{
}

void SkinHeadNPC::Update(float deltaTime)
{
	if (GetCurState() == State::Credit)
	{
		m_TargetPos = { 1900, 600 };
		m_topPos = { 1500, -100 };
		m_myPos = m_transform->GetLocalPosition();
		
		//m_transform->SetTransform(m_directY);

		if (isCreditFly)
		{
			m_startPos = m_myPos;
			isCreditFly = false;
		}

		m_transform->SetLocalPosition(MoveParabolic(deltaTime));

		m_degree += deltaTime * 10;
		if (m_degree > 360)
		{
			m_degree = 0;
		}

		m_transform->Rotate(m_degree);

		/// 포물선 스케일변화
		// m_topPos.x까지는 스케일이 증가함
		if (m_myPos.x <= m_topPos.x)
		{
			float midScale = 1.f + 0.5f - (((m_topPos.x - m_myPos.x) / (m_topPos.x - m_startPos.x)) / 2.f);
			m_transform->SetLocalScale({ midScale,midScale });
		}
	}

	if (m_curState == State::Credit)
	{
		return;
	}

	m_time += deltaTime;
	if (m_time > 1)
	{
		m_index++;

		if (m_index >= 3)
		{
			m_index = 0;
		}

		m_time = 0;
	}

	// 차에 치였을때 도착하는 곳
	m_TargetPos = WaitingLinePoint((float)(1650.f - (180.f / 3) * 3));
	
	// 카메라가 사라졌을 떄의 위치를 저장하기 위한변수
	D2D1_MATRIX_3X2_F accidentMatrix = m_transform->GetTransformMatrix() * CameraManager::GetInstance().GetCameraMatrix();
	Collider* myCollider = ((Collider*)(m_gameObject->GetElement(ElementType::Collider)));

	/// 충돌했을 경우
	if (myCollider->GetIsCollision() && isGoingHome == false && isAccident == false/* && isInterviewExit==false*/)
	{
		m_audioSource->PlayBack("sfx_hit_npc_1");
		// 충돌상태에 들어간다.
		SetIsAccident(true);
		
		// 카메라를 사용하지 않고
		m_transform->SetUseCamera(false);

		// 로컬위치를 0으로 옮긴다.
		m_transform->SetLocalPosition({ 0,0 });

		// 카메라 사용전의 위치와 사용 후의 위치를 일치 시킨다.
		m_myPos = m_directY->MarixToVector(accidentMatrix, m_transform->GetLocalPosition());

		/// 충돌이 시작되면 스타트 포즈를 저장한다.
		m_startPos = m_myPos;

		// 카메라해제 후에 같은 위치에 있을 수 있도록 포지션을 옮긴다.
		m_transform->SetLocalPosition(m_myPos);

		// 레이어를 높여서 화면상 가장 위에 표시되도록 옮긴다.
		m_transform->SetLayer(400);
	}

	/// 이동
	// 충돌상태라면 && 정해진 x포즈까지 && 집으로 돌아가는게 아닐경우
	// ui메뉴 방향(타겟포즈)으로 이동한다.
	if (GetIsAccident() && (m_myPos.x <= m_TargetPos.x) && !isGoingHome)
	{
		// 포물선으로 움직인다.
		m_transform->SetLocalPosition(MoveParabolic(deltaTime));
		m_myPos = m_transform->GetLocalPosition();
	}
	/// 집으로 돌아가야 하는경우
	else if (isGoingHome == true && ((int)(m_myPos.x*100)/1000 != (int)(m_paths[0].x*100)/1000))
	{
		// 포물선으로 움직인다.
		m_transform->SetLocalPosition(MoveReverseParabolic(deltaTime));
		m_myPos = m_transform->GetLocalPosition();
		m_transform->Rotate(m_myPos.x);
	}
	else if(isGoingHome)
	{
		m_transform->SetLocalPosition(m_paths[0]);
		m_transform->SetLocalRotation(0);
		isGoingHome = false;
		ChangeState(State::Idle);
		m_transform->SetLayer(5);

		/// 티켓을 초기화하고 받는 위치는 다시 생각해보자
		SetWaitNumber(-1);
		isAccident = false;
		isInterview = false;
	}

	switch (GetCurState())
	{
	case SkinHeadNPC::State::Idle:
		// 새로운 길을 찾고 Walk로 변경!
		// 이렇게 하는게 맞는지는 모르겠지만 WalkingNPCState에서 SetNextDestination으로 바꾸기는 함.
		// 아니 안함
		SetNextDestination();
		//PathFind();
		ChangeState(State::Walk);
		break;

	case SkinHeadNPC::State::Walk:
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
	case SkinHeadNPC::State::Attacked:
		// 차에 부딫히면 오른쪽 UI로 날려보내기!
		break;
	case SkinHeadNPC::State::Interview:
		// 대기번호 1~4f를 받고
		// ui에 도착하면 인터뷰 대기상태가 됩니다! 

		ChangeState(State::Interview);
		break;
	case SkinHeadNPC::State::Credit:

		break;
	default:
		break;
	}
	m_FSM->UpdateState(deltaTime);

	/*--------------------------------------------------------------------------------------*/
	if (isWaiting)
	{
		isInterview = true;

		switch (waitNumber)
		{
		case (1):
			m_currentWaitingPoint = WaitingLinePoint(1650.f);
			break;
		case (2):
			m_currentWaitingPoint = WaitingLinePoint((float)(1650.f - (180.f / 3)));
			break;
		case (3):
			m_currentWaitingPoint = WaitingLinePoint((float)(1650.f - (180.f / 3) * 2));
			break;
		default:
			m_currentWaitingPoint = WaitingLinePoint((float)(1650.f - (180.f / 3) * 3));
			break;
		}

		m_transform->SetLocalPosition(m_currentWaitingPoint);
		m_curDirState = Direction::Up;
		SetAnimation();
	}
	/*--------------------------------------------------------------------------------------*/

	// 면접이 끝난경우
	if (isInterview && waitNumber == -1&& !m_isHero)
	{
		waitNumber = 5;
		//m_gameObject->SetIsDraw(true);
		isInterview = false;
		isAccident = true;
	}
	if (isInterview && waitNumber == -1 && m_isHero)
 	{
  		//m_transform->SetLocalScale({ 2.f,2.f });
  		m_gameObject->SetActive(false);
 		//waitNumber = 5;
  		//isInterview = false;
 		//isAccident = true;
 	}

	// 회전을 넣자
	// 회전은 충돌상태 + m_myPos.x가 1500 이전일때 
	// 회전각도가 360 or 0 가 아닐 때 까지 한다
	if (GetIsAccident())
	{
		// 각도를 계속 가산해준다.
		m_degree += deltaTime * 10;
		if (m_degree > 360)
		{
			m_degree = 0;
		}

		/// 포물선 스케일변화
		// m_topPos.x까지는 스케일이 증가함
		if (m_myPos.x <= m_topPos.x)
		{
			float midScale = 1.f + 0.5f - (((m_topPos.x - m_myPos.x) / (m_topPos.x - m_startPos.x)) / 2.f);
			m_transform->SetLocalScale({midScale,midScale});
		}
		// m_topPos.x - m_TargetPos.x까지는 스케일이 감소함
		else if (m_myPos.x > m_topPos.x)
		{
			float endScale = 1.f + 0.5f - (1.f - (m_TargetPos.x - m_myPos.x) / (m_TargetPos.x - m_topPos.x));
			m_transform->SetLocalScale({endScale,endScale});
		}

		// x값이 목표지점에 도달하면 회전을 멈춘다.
		// 목표지점에서의 스케일을 고정한다.
		if ((m_myPos.x >= m_TargetPos.x /*&& isInterviewExit != true*/)/*&&isWaiting*/)
		{
			SetIsAccident(false);
			m_degree = 0; 
			m_transform->SetLocalRotation(0);
			m_transform->SetLocalScale({0.5f,0.5f});
			
			// 대기표를 받는다.
			if (waitNumber == -1)
			{
				isWaiting = true;
				SetTakeTicket(true);
			}
		}

		// 대기열에서 벗어나면 원래자리로 돌아간다.
		if ((isAccident==false) && (waitNumber == 5) && m_startPos.x < m_myPos.x)
		{
			// 카메라 사용
			m_transform->SetUseCamera(true);
			// 카메라메트릭스 만큼 빼주기
			m_myPos = m_transform->GetLocalPosition() - CameraManager::GetInstance().GetCameraPosition();
			// 카메라를 사용했지만 현재위치에 도달
			m_transform->SetLocalPosition(m_myPos);

			m_transform->SetLocalScale({0.5,0.5});
			MakeReverseTopPos(m_myPos, m_paths[0]);
			
			//pathIndex를 바꿔줘야 도착해서 잘 움직임
			m_pathIndex = 0;
			isGoingHome = true;
			isAccident = false;
			isWaiting = false;
			//isInterview = false;

			isInterviewExit = false;
		}

		m_transform->Rotate(m_degree);
	}

}

void SkinHeadNPC::LateUpdate()
{
}

void SkinHeadNPC::PreRender()
{
	switch (GetCurState())
	{
	case State::Idle:
		break;
	case State::Walk:
		FindThisPositionAndSetLayer();
		break;
	case State::Attacked:
		break;
	case State::Interview:
		m_transform->SetLayer(100000);
		break;
	default:
		break;

	}
}

void SkinHeadNPC::Render(DirectY* directY)
{
	m_directY = directY;
}

void SkinHeadNPC::PostRender(DirectY* directY)
{
}

void SkinHeadNPC::Release()
{
}

ElementType SkinHeadNPC::GetElementType() const
{
	return ElementType::Script;
}

GameObject* SkinHeadNPC::GetGameObject() const
{
	return m_gameObject;
}

void SkinHeadNPC::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}



void SkinHeadNPC::PrintLog(std::wstring log, bool isNpc)
{
	m_printLog(log,isNpc);
}

/// <summary>
/// 포물선이동을 위한 함수
/// </summary>
/// <param name="deltaTime"></param>
/// <returns></returns>
MathEngine::Vector2 SkinHeadNPC::MoveParabolic(float deltaTime)
{
	float A = (float)(m_TargetPos.y * (pow(m_myPos.x, 2) - pow(m_topPos.x, 2))
		+ m_topPos.y * (pow(m_TargetPos.x, 2) - pow(m_myPos.x, 2))
		+ m_myPos.y * (pow(m_topPos.x, 2) - pow(m_TargetPos.x, 2)))
		/ (2 * ((m_TargetPos.y - m_topPos.y) * m_myPos.x
			+ (m_myPos.y - m_TargetPos.y) * m_topPos.x
			+ (m_topPos.y - m_myPos.y) * m_TargetPos.x));
	
	float C = (float)(m_myPos.y - m_topPos.y) / (float)((pow(m_myPos.x, 2) - pow(m_topPos.x, 2) + 2 * A * (m_topPos.x - m_myPos.x)));
	
	float B = m_topPos.y - (float)(C * pow((m_topPos.x - A),2));

	m_myPos.x += deltaTime* 200;

	float y = (float)(C * pow((m_myPos.x - A), 2) + B);


	return { m_myPos.x,y };
}
/// <summary>
/// 제자리로 돌아가는 함수
/// </summary>
/// <param name="deltaTime"></param>
/// <returns></returns>
MathEngine::Vector2 SkinHeadNPC::MoveReverseParabolic(float deltaTime)
{
	float A = (float)(m_paths[0].y * (pow(m_myPos.x, 2) - pow(m_reverseTopPos.x, 2))
		+ m_reverseTopPos.y * (pow(m_paths[0].x, 2) - pow(m_myPos.x, 2))
		+ m_myPos.y * (pow(m_reverseTopPos.x, 2) - pow(m_paths[0].x, 2)))
		/ (2 * ((m_paths[0].y - m_reverseTopPos.y) * m_myPos.x
			+ (m_myPos.y - m_paths[0].y) * m_reverseTopPos.x
			+ (m_reverseTopPos.y - m_myPos.y) * m_paths[0].x));

	float C = (float)(m_myPos.y - m_reverseTopPos.y) / (float)((pow(m_myPos.x, 2) - pow(m_reverseTopPos.x, 2) + 2 * A * (m_reverseTopPos.x - m_myPos.x)));

	float B = m_reverseTopPos.y - (float)(C * pow((m_reverseTopPos.x - A), 2));

	if(m_myPos.x >= m_paths[0].x)
	{
		m_myPos.x -= deltaTime * 200;
	}
	else if (m_myPos.x < m_paths[0].x)
	{
		m_myPos.x += deltaTime * 200;
	}

	float y = (float)(C * pow((m_myPos.x - A), 2) + B);


	return { m_myPos.x, y };
}

MathEngine::Vector2 SkinHeadNPC::MoveToUI(float deltaTime)
{
	const int intensity = 600;
	// 시간이 지날때마다 x값이 증가한다.
	float a = (m_TargetPos.y - m_myPos.y) / (m_TargetPos.x - m_myPos.x);
	m_myPos += deltaTime;

	return { 1 * deltaTime * intensity, a * deltaTime * intensity };
}

MathEngine::Vector2 SkinHeadNPC::WaitingLinePoint(float x)
{
	float y = -0.604f * x + 1795.f;
	return { x,y };
}

/// <summary>
/// 돌아갈는 포물선의 중간지점을 만드는 함수
/// </summary>
/// <param name="myPos">시작점</param>
/// <param name="targetPos">도착점</param>
void SkinHeadNPC::MakeReverseTopPos(MathEngine::Vector2 myPos, MathEngine::Vector2 targetPos)
{
	if (myPos.x >= targetPos.x)
	{
		m_reverseTopPos.x = targetPos.x + (myPos.x - targetPos.x)/2;
	}
	else if (myPos.x < targetPos.x)
	{
		m_reverseTopPos.x = myPos.x + (targetPos.x - myPos.x)/2;
	}

	if (myPos.y >= targetPos.y)
	{
		m_reverseTopPos.y = targetPos.y - 500;
	}
	else if (myPos.y < targetPos.y)
	{
		m_reverseTopPos.y = targetPos.y - 500;
	}
}

void SkinHeadNPC::SetNextDestination()
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
		m_pathIndex--;
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
	SetAnimation();
}

bool SkinHeadNPC::IsArrive()
{
	// TODO : 현재 도착했는지를 검사!
	// 오차범위를 꽤 크게 잡는데 이게 맞는지 모르겠다. 계속 걸어댕기다 보면 오차범위가 생길 것 같음. 
	Vector2 currentPos = m_transform->GetLocalPosition();
	if (m_currentDestination.x + 10.f >= currentPos.x
		&& m_currentDestination.x - 10.f <= currentPos.x
		&& m_currentDestination.y + 10.f >= currentPos.y
		&& m_currentDestination.y - 10.f <= currentPos.y)
	{
		//m_transform->SetLocalPosition(m_currentDestination);
		return true;
	}

	return false;
}

bool SkinHeadNPC::IsAttacked()
{
	// TODO : 차에 치였는지를 검사!
	return isAccident;
}

void SkinHeadNPC::Move(float deltaTime)
{
	m_transform->Translate(m_currentDirection * m_speed * deltaTime);
}

void SkinHeadNPC::MoveInUI(float deltaTime)
{
	m_transform->Translate(m_currentWaitingPoint * m_speed * deltaTime);
}

void SkinHeadNPC::ChangeState(State nextState)
{
	m_curState = nextState;
	switch (GetCurState())
	{
	case SkinHeadNPC::State::Idle:
		m_FSM->ChangeState(new WalkingNPCIdleState(this));
		break;
	case SkinHeadNPC::State::Walk:
		m_FSM->ChangeState(new WalkingNPCWalkState(this));
		break;
	case SkinHeadNPC::State::Attacked:
		m_FSM->ChangeState(new WalkingNPCAttackedState(this));
		break;
	case SkinHeadNPC::State::Interview:
		//m_FSM->ChangeState(new WalkingNPCAttackedState(this));
		break;
	case SkinHeadNPC::State::Credit:
		//m_FSM->ChangeState(new WalkingNPCAttackedState(this));
		break;
	default:
		break;
	}
}

void SkinHeadNPC::SetNextDirection()
{
	VectorInt currentPos = m_pathArr[m_beforePathIndex];
	VectorInt nextPos = m_pathArr[m_pathIndex];

	if (currentPos.x == nextPos.x)
	{
		if (currentPos.y <= nextPos.y)
		{
			m_curDirState = Direction::Down;
		}
		else
		{
			m_curDirState = Direction::Up;
		}
	}
	else if (currentPos.y == nextPos.y)
	{
		if (currentPos.x <= nextPos.x)
		{
			m_curDirState = Direction::Right;
		}
		else
		{
			m_curDirState = Direction::Left;
		}
	}
	else
	{
		m_curDirState;
	}
}

void SkinHeadNPC::SetNextDirectionInUI()
{
	if (m_myPos.x == m_currentWaitingPoint.x)
	{
		if (m_myPos.y <= m_currentWaitingPoint.y)
		{
			m_curDirState = Direction::Down;
		}
		else
		{
			m_curDirState = Direction::Up;
		}
	}
	else if (m_myPos.y == m_currentWaitingPoint.y)
	{
		if (m_myPos.x <= m_currentWaitingPoint.x)
		{
			m_curDirState = Direction::Right;
		}
		else
		{
			m_curDirState = Direction::Left;
		}
	}
	else
	{
		m_curDirState;
	}
}

void SkinHeadNPC::SetAnimation()
{
	switch (m_curDirState)
	{
	case Direction::Up:
		m_animator->SetCurrentMotion("_back_right_walking");
		break;
	case Direction::Right:
		m_animator->SetCurrentMotion("_front_right_walking");
		break;
	case Direction::Down:
		m_animator->SetCurrentMotion("_front_left_walking");
		break;
	case Direction::Left:
		m_animator->SetCurrentMotion("_back_left_walking");
		break;
	default:
		break;
	}
}

void SkinHeadNPC::FindThisPositionAndSetLayer()
{
	// 배열 좌표, 보정된 위치
	std::pair<MathEngine::Vector2, MathEngine::Vector2> getData
		= m_findPosition->FindNearest(m_transform->GetLocalPosition());

	m_currentLayer = m_layerArr[getData.first.x][getData.first.y];

	// 여기서 레이어 설정
	m_transform->SetLayer(m_currentLayer + 1);
}
