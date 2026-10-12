#pragma once
#include "NPC.h"
#include <vector>
#include "Vector2.h"
#include <functional>
#include <string>

namespace Graphics { struct Sprite; }
namespace Graphics { class Motion; }
namespace Algorithm { class FindPosition; }

class Transform;
class FSM;
class Animator;

struct CarVectorInt
{
	int x, y;
};

class Transform;
class CarNpc : public NPC
{
public:
	CarNpc(Transform* transform);
	virtual ~CarNpc();
	enum class State
	{
		Idle,
		Move,
		Attacked,
	};

	enum class Direction
	{
		Up,
		Right,
		Down,
		Left,

		Count,
	};

	virtual void Init() override;

	virtual void FixedUpdate() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate() override;

	virtual void PreRender() override;
	virtual void Render(DirectY* directY) override;
	virtual void PostRender(DirectY* directY) override;

	virtual void Release() override;

	virtual ElementType GetElementType() const override;

	virtual GameObject* GetGameObject() const override;
	virtual void SetGameObject(GameObject* gameObject) override;

	std::vector<MathEngine::Vector2>& GetPaths() { return m_paths; }

private:
	DirectY* m_directY;

	// gyu code!
public:
	/// <summary>
	/// 다음 목적지로 세팅
	/// </summary>
	void SetNextDestination();

private:
	bool IsArrive();
	bool IsAttacked();

	/// <summary>
	/// 현재 목적지로 한 번씩 이동
	/// </summary>
	void Move(float deltaTime);
	void MoveInUI(float deltaTime);

public:
	void AddPath(MathEngine::Vector2 pos) { GetPaths().push_back(pos); }

private:
	void ChangeState(State nextState);

	//Graphics::Motion* m_motion;
	Transform* m_transform;

	State m_curState;
	FSM* m_FSM;
	friend class CarNPCIdleState;
	friend class CarNPCMoveState;
	friend class CarNPCAttackedState;

	/// 목적지 관련
	// 현재 목적지
	MathEngine::Vector2 m_currentDestination;
	// 여러 개의 목적지를 보관
	std::vector<MathEngine::Vector2> m_destinations;

	// 현재 목적지까지 가기 위한 경로의 포지션들
	std::vector<MathEngine::Vector2> m_paths;
	// 현재 경로 인덱스
	int m_pathIndex = 0;

	// 목적지 개수
	int m_destinationCount;
	// 다음 목적지로 가기 위해 필요한 인덱스
	int m_destinationIndex;
	// 반대 방향으로 갈지 정함.
	bool isReverse = false;
	bool m_isHero;

	float m_speed = 200.f;
	MathEngine::Vector2 m_currentDirection = {};
	// 대기위치
	MathEngine::Vector2 m_currentWaitingPoint = {};

public:
	// npc의 경로를 포지션으로 변환하기 전에 추가
	void AddPathArr(int posX, int posY) { m_pathArr.push_back({ posX, posY }); }

	int GetCarID() const { return m_carID; }
	void SetCarID(int val) { m_carID = val; }

private:
	void SetNextDirection();

	// npc 애니메이션의 방향을 설정하기 위해 Direction을 가짐.
	Direction m_curDirState = {};
	// npc의 다음 위치의 방향을 가지고있는 배열로 설정
	std::vector<CarVectorInt> m_pathArr;

	int m_beforePathIndex = 0;
	int m_carID;

public:
	void SetMotion(Graphics::Motion* motion) { m_motion = motion; }
private:
	int m_npcId;

	// 모션
	Graphics::Motion* m_motion;
	int m_spriteIndex;

	MathEngine::Vector2 m_preDirection;
	MathEngine::Vector2 m_direction;

	float m_time = 0.f;

	// 레이어 찾기
		// 현재의 위치에 따라 레이어를 바꾸기 위한 함수를 만들 예정
	void FindThisPositionAndSetLayer();
	Algorithm::FindPosition* m_findPosition;
	std::vector<std::vector<int>> m_layerArr
		= std::vector<std::vector<int>>(100, std::vector<int>(100));
	int m_tileLayer;
	int m_size;
	int m_currentLayer;
};

