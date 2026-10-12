#include "WalkingNPCState.h"

#pragma region IdleState
void WalkingNPCIdleState::OnStateEnter()
{
	//WalkingNPC* npc = (WalkingNPC*)m_NPC;

	//// 처음 목적지를 설정
	//npc->SetNextDestination();
}

void WalkingNPCIdleState::OnStateUpdate(float deltaTime)
{
	
}

void WalkingNPCIdleState::OnStateRender(DirectY* directY)
{

}

void WalkingNPCIdleState::OnStateExit()
{

}
#pragma endregion IdleState

#pragma region WalkState
void WalkingNPCWalkState::OnStateEnter()
{

}

void WalkingNPCWalkState::OnStateUpdate(float deltaTime)
{

}

void WalkingNPCWalkState::OnStateRender(DirectY* directY)
{

}

void WalkingNPCWalkState::OnStateExit()
{

}
#pragma endregion WalkState

#pragma region AttackedState
void WalkingNPCAttackedState::OnStateEnter()
{

}

void WalkingNPCAttackedState::OnStateUpdate(float deltaTime)
{

}

void WalkingNPCAttackedState::OnStateRender(DirectY* directY)
{

}

void WalkingNPCAttackedState::OnStateExit()
{

}
#pragma region AttackedState

void WalkingNPCInterviewState::OnStateEnter()
{

}

void WalkingNPCInterviewState::OnStateUpdate(float deltaTime)
{

}

void WalkingNPCInterviewState::OnStateRender(DirectY* directY)
{

}

void WalkingNPCInterviewState::OnStateExit()
{

}
