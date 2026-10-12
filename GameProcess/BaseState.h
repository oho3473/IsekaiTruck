#pragma once

class NPC;
class DirectY;

class BaseState
{
public:
	virtual void OnStateEnter() abstract;
	virtual void OnStateUpdate(float deltaTime) abstract;
	virtual void OnStateRender(DirectY* directY) abstract;
	virtual void OnStateExit() abstract;

	virtual ~BaseState() {}

protected: 
	BaseState(NPC* npc) : m_NPC(npc) {}

	NPC* m_NPC;
};

