#include "FSM.h"
#include "BaseState.h"

#include "DirectY.h"

FSM::FSM(BaseState* baseState)
{
	m_curState = baseState;
	ChangeState(m_curState);
}

FSM::~FSM()
{
	delete m_curState;
}

void FSM::ChangeState(BaseState* nextState)
{
	if (nextState == m_curState)
	{
		return;
	}

	if (m_curState != nullptr)
	{
		m_curState->OnStateExit();
		delete m_curState;
	}

	m_curState = nextState;
	m_curState->OnStateEnter();
}

void FSM::UpdateState(float deltaTime)
{
	if (m_curState != nullptr)
	{
		m_curState->OnStateUpdate(deltaTime);
	}
}

void FSM::RenderState(DirectY* directY)
{
	if (m_curState != nullptr)
	{
		m_curState->OnStateRender(directY);
	}
}
