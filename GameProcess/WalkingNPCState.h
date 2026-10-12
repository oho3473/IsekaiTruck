#pragma once
#include "BaseState.h"

class WalkingNPCIdleState : public BaseState
{
public:
	WalkingNPCIdleState(NPC* npc) : BaseState(npc) {}

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;
};

class WalkingNPCWalkState : public BaseState
{
public:
	WalkingNPCWalkState(NPC* npc) : BaseState(npc) {}

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;
};

class WalkingNPCAttackedState : public BaseState
{
public:
	WalkingNPCAttackedState(NPC* npc) : BaseState(npc) {}

	virtual void OnStateEnter() override; 
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;
};

class WalkingNPCInterviewState : public BaseState
{
public:

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;

};