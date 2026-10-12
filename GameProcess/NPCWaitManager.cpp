#include "NPCWaitManager.h"
#include "SkinHeadNPC.h"
#include "NpcManager.h"

NPCWaitManager::NPCWaitManager(NpcManager* npcManager)
	: m_npcManager(npcManager), m_npcList{}, m_waitQueue{}
{
	// 모든 npc의 정보를 받아온다.
	m_npcList = m_npcManager->GetNpcs();
}

NPCWaitManager::~NPCWaitManager()
{
	
}

void NPCWaitManager::Init()
{

}

void NPCWaitManager::FixedUpdate()
{

}

void NPCWaitManager::Update(float deltaTime)
{
	CheckCollision();
}

void NPCWaitManager::LateUpdate()
{

}

void NPCWaitManager::PreRender()
{

}

void NPCWaitManager::Render(DirectY* directY)
{

}

void NPCWaitManager::PostRender(DirectY* directY)
{

}

void NPCWaitManager::Release()
{

}

ElementType NPCWaitManager::GetElementType() const
{
	return ElementType::Script;
}

GameObject* NPCWaitManager::GetGameObject() const
{
	return m_gameObject;
}

void NPCWaitManager::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void NPCWaitManager::SetWaitNumber(SkinHeadNPC* waitingNpc)
{
	waitingNpc->SetWaitNumber(m_npcList.size());
}

/// <summary>
/// npc의 충돌상태를 확인하고
/// 충돌했을 경우 queue에 추가,
/// 대기번호를 추가한다.
/// </summary>
void NPCWaitManager::CheckCollision()
{
	static int num = 0;
	// npc의 충돌상태를 확인한다.
	for (int i = 0; i < (int)m_npcList.size(); i++)
	{
		// npc가 충돌한경우
		if (m_npcList[i]->GetTakeTicket())
		{
			// 대기하고 있는 npc가 4개 미만일 경우
			if(m_waitQueue.size() < 4)
			{
				// 대기열에 해당 npc를 추가하고 
				// 대기넘버를 부여함
				m_waitQueue.push(m_npcList[i]);
				m_npcList[i]->SetWaitNumber(m_waitQueue.size());
			}
			// 대기 중인 npc가 4개 이상일 경우
			else
			{
				// 대기열에 추가하지 않음
				// number5를 부여함
				m_npcList[i]->SetWaitNumber(5);
				m_npcList[i]->SetIsAccident(true);
			}
		}
		m_npcList[i]->SetTakeTicket(false);
	}
}

void NPCWaitManager::isPoP()
{
	// npc의 대기상태를 확인한다.
	for (int i = 0; i < (int)m_npcList.size(); i++)
	{
		// npc가 대기중인 경우
		if (m_npcList[i]->GetIsWaiting())
		{
			// 맨 앞일 때
			if(m_npcList[i]->GetWaitNumber() == 1)
			{
				m_npcList[i]->SetWaitNumber(-1);
				m_npcList[i]->SetIsInterview(true);

				m_npcList[i]->SetIsWaiting(false);
				m_npcList[i]->SetIsInterviewExit(true);
			}
			else if (m_npcList[i]->GetWaitNumber() > 1 && m_npcList[i]->GetWaitNumber() <= 4)
			{
				m_npcList[i]->SetWaitNumber(m_npcList[i]->GetWaitNumber() - 1);
			}
		}
	}
}
