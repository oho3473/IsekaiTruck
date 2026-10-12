#pragma once
#include "NPC.h"
//#include "ElementBase.h"
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
class AudioSource;
namespace Algorithm { class PathFinding; }

struct VectorInt
{
	int x, y;
};

class SkinHeadNPC : public NPC
{
public:
	SkinHeadNPC() {}
	SkinHeadNPC(Transform* npcTransform);
	~SkinHeadNPC();

	enum class State
	{
		Idle,
		Walk,
		Attacked,

		// 면접상태를 만들어보자
		Interview,
		Credit,
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

	void SetPrintLog(std::function<void(std::wstring,bool)> printLog) { m_printLog = printLog; }
	void PrintLog(std::wstring log, bool isNpc);

	MathEngine::Vector2 MoveReverseParabolic(float deltaTime);
	MathEngine::Vector2 MoveParabolic(float deltaTime);
	MathEngine::Vector2 MoveToUI(float deltaTime);
	MathEngine::Vector2 WaitingLinePoint(float x);

	//test
	Graphics::Motion* m_motion;
	int m_index;

	int GetWaitNumber() const { return waitNumber; }
	void SetWaitNumber(int val) { waitNumber = val; }

	bool GetIsAccident() const { return isAccident; }
	void SetIsAccident(bool val) { isAccident = val; }

	bool GetTakeTicket() const { return takeTicket; }
	void SetTakeTicket(bool val) { takeTicket = val; }
	std::vector<MathEngine::Vector2>& GetPaths() { return m_paths; }

	// 돌아가는 중점 찍어주기
	void MakeReverseTopPos(MathEngine::Vector2 myPos, MathEngine::Vector2 targetPos);
	
	bool GetIsWaiting() const { return isWaiting; }
	void SetIsWaiting(bool val) { isWaiting = val; }

	bool GetIsgoddess() const { return isgoddess; }
	void SetIsgoddess(bool val) { isgoddess = val; }
	bool GetIsInterview() const { return isInterview; }
	void SetIsInterview(bool val) { isInterview = val; }
	bool GetIsInterviewExit() const { return isInterviewExit; }
	void SetIsInterviewExit(bool val) { isInterviewExit = val; }
	void SetIsCreditFly(bool val) { isCreditFly = val; }
	SkinHeadNPC::State GetCurState() const { return m_curState; }
private:
	float m_sumTime;
	float m_degree;
	float m_targetScale;

	float m_time;

	// 대기열을 위한 인트변수
	int waitNumber;

	bool takeTicket = false;
	bool isGoingHome = false;
	bool isAccident;
	bool isWaiting = false;
	bool isInterview = false;
	bool isInterviewExit = false;

	bool isCreditFly = false;
	DirectY* m_directY;
	AudioSource* m_audioSource;

	MathEngine::Vector2 m_myPos;
	MathEngine::Vector2 m_topPos;
	MathEngine::Vector2 m_TargetPos;
	MathEngine::Vector2 m_startPos;
	MathEngine::Vector2 m_reverseTopPos;

	std::function<void(std::wstring,bool)> m_printLog;

	// gyu code!
public:
	/// <summary>
	/// 다음 목적지로 세팅
	/// </summary>
	void SetNextDestination();

	void SetAudioSource(AudioSource* audioSouce) { m_audioSource = audioSouce; };

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

	void SetHero(bool val) { m_isHero = val; }
	bool GetHero() { return m_isHero; }
	void ChangeState(State nextState);
private:
	Algorithm::PathFinding* m_pathFind;
	//Graphics::Motion* m_motion;
	Transform* m_transform;

	State m_curState;
	FSM* m_FSM;
	friend class WalkingNPCIdleState;
	friend class WalkingNPCMWalkState;
	friend class WalkingNPCAttackedState;

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
	void AddPathArr(int posX, int posY) { m_pathArr.push_back({posX, posY}); }

	Animator* GetAnimator() { return m_animator; }
	void SetAnimator(Animator* animator) { m_animator = animator; }

	int GetNpcID() const { return m_npcID; }
	void SetNpcID(int val) { m_npcID = val; }

private:
	void SetNextDirection();
	// ui안에서 움직임
	void SetNextDirectionInUI();
	void SetAnimation();

	// npc 애니메이션의 방향을 설정하기 위해 Direction을 가짐.
	Direction m_curDirState = {};
	// npc의 다음 위치의 방향을 가지고있는 배열로 설정
	std::vector<VectorInt> m_pathArr;

	int m_beforePathIndex = 0;

	int m_npcID;
	// 애니메이션!
	Animator* m_animator = nullptr;

	bool isgoddess;

	/// Gyu code
	// 현재의 위치에 따라 레이어를 바꾸기 위한 함수를 만들 예정
	void FindThisPositionAndSetLayer();
	Algorithm::FindPosition* m_findPosition;
	std::vector<std::vector<int>> m_layerArr
		= std::vector<std::vector<int>>(100, std::vector<int>(100));
	int m_tileLayer;
	int m_size;
	int m_currentLayer;
};