#pragma once

class BaseState;
class DirectY;

class FSM
{
public:
	FSM(BaseState* baseState);
	~FSM();

	void ChangeState(BaseState* nextState);
	void UpdateState(float deltaTime);
	void RenderState(DirectY* directY);

private:
	BaseState* m_curState;
};

