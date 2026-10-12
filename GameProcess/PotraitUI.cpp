#include "PotraitUI.h"
#include "SkinHeadNPC.h"
#include "DirectY.h"

PotraitUI::PotraitUI()
{

}

PotraitUI::~PotraitUI()
{
	delete m_potraitGroup;
}

void PotraitUI::Init()
{

}

void PotraitUI::FixedUpdate()
{

}

void PotraitUI::Update(float deltaTime)
{

}

void PotraitUI::LateUpdate()
{

}

void PotraitUI::PreRender()
{

}

void PotraitUI::Render(DirectY* directY)
{
	if (!m_interviewlist->empty() && m_interviewlist->front()->GetIsWaiting())
	{
		int NPC = NPC = (m_interviewlist->front())->GetNpcID();
		
		directY->DrawBitmap(m_potraitGroup->GetSprite(NPC - 1));
	}
}

void PotraitUI::PostRender(DirectY* directY)
{

}

void PotraitUI::Release()
{

}

ElementType PotraitUI::GetElementType() const
{
	return ElementType::Script;
}

GameObject* PotraitUI::GetGameObject() const
{
	return m_gameObject;
}

void PotraitUI::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
