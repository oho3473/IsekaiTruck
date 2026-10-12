#include "TargetBar.h"
#include "SkinHeadNPC.h"
#include "DirectY.h"
#include "NpcManager.h"
#include "MessageManager.h"
#include "NPCWaitManager.h"

TargetBar::TargetBar(MessageManager* messagemanager, NpcManager* npcmanager, NPCWaitManager* npcwaitmanager) : m_npcList(npcmanager), m_messageManager(messagemanager),m_npcWaitManager(npcwaitmanager), m_ClearCount(0)
{

}

TargetBar::~TargetBar()
{
}

void TargetBar::Init()
{
	m_ClearCount = m_messageManager->GetClearCount();

	int i = 0;
	for (auto e : m_npcList->GetNpcs())
	{
		if (e->GetHero())
		{
			m_answerList.push_back(e);
			i++;
		}
	}

	for (int i = 0; i < m_ClearCount; i++)
	{
		m_useList.push_back(false);
	}

}

void TargetBar::FixedUpdate()
{

}

void TargetBar::Update(float deltaTime)
{
	static float time = 0;

	m_interViewList = m_npcWaitManager->GetWaitQueue();

	if (!m_interViewList.empty())
	{
		if (!m_useList.empty())
		{

			for (int i = 0; i < m_ClearCount; i++)
			{
				if (m_interViewList.front()->GetHero())
				{
					if (m_answerList[i]->GetNpcID() == m_interViewList.front()->GetNpcID())
					{
						m_useList[i] = true;
					}
				}
			}
		}
	}

}

void TargetBar::LateUpdate()
{

}

void TargetBar::PreRender()
{

}

void TargetBar::Render(DirectY* directY)
{
	directY->DrawBitmap(m_backGround->GetSprite(0));

	if (!m_setpos)
	{
		for (int i = 0; i < m_ClearCount; i++)
		{
			m_image->GetSprite((m_answerList[i]->GetNpcID() - 1))->m_drawPos.x += 25;
			m_image->GetSprite((m_answerList[i]->GetNpcID() - 1))->m_drawPos.x += 80*i;
			m_image->GetSprite((m_answerList[i]->GetNpcID() - 1))->m_drawPos.y += 30; 
		}

		m_setpos = true;
	}

	for (int i = 0; i < m_ClearCount; i++)
	{
		if (m_useList[i])
		{
			//정답 반투명 처리
			directY->DrawBitmap(m_image->GetSprite(m_answerList[i]->GetNpcID() - 1));
			m_tonedown->GetSprite(0)->m_drawPos = m_image->GetSprite((m_answerList[i]->GetNpcID() - 1))->m_drawPos;
			directY->DrawBitmap(m_tonedown->GetSprite(0));
		}
		else
		{
			
			directY->DrawBitmap(m_image->GetSprite(m_answerList[i]->GetNpcID() - 1));
		}
	}
}

void TargetBar::PostRender(DirectY* directY)
{

}

void TargetBar::Release()
{

}

ElementType TargetBar::GetElementType() const
{
	return ElementType::Script;
}

GameObject* TargetBar::GetGameObject() const
{
	return m_gameObject;
}

void TargetBar::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
