#pragma once

#include "BaseState.h"
#include "Vector2.h"

#include <vector>

namespace Algorithm { class	PathFinding; }

class PoliceIdleState : public BaseState
{
public:
	PoliceIdleState(NPC* npc);
	~PoliceIdleState();

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;

private:
	std::vector<MathEngine::Vector2> m_route;
	int m_index;
};

class PoliceChasingState : public BaseState
{
public:
	PoliceChasingState(NPC* npc) 
		: BaseState(npc)
		, m_objectLocation(MathEngine::Vector2{0.f, 0.f}) {}

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;

private:
	MathEngine::Vector2 m_objectLocation;
};

class PoliceCollisionState : public BaseState
{
public:
	PoliceCollisionState(NPC* npc);
	~PoliceCollisionState();

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;

private:
};