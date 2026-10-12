#pragma once
#include "BaseState.h"

class CarNPCIdleState : public BaseState
{
public:
	CarNPCIdleState(NPC* npc) : BaseState(npc) {}

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;
};

class CarNPCMoveState : public BaseState
{
public:
	CarNPCMoveState(NPC* npc) : BaseState(npc) {}

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;
};

class CarNPCAttackedState : public BaseState
{
public:
	CarNPCAttackedState(NPC* npc) : BaseState(npc) {}

	virtual void OnStateEnter() override;
	virtual void OnStateUpdate(float deltaTime) override;
	virtual void OnStateRender(DirectY* directY) override;
	virtual void OnStateExit() override;
};