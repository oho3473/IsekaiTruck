#include "InterviewUI.h"


#include "GameObject.h"
#include "Transform.h"

#include "InputManager.h"

#include "Sprite.h"
#include "DirectY.h"

InterviewUI::InterviewUI()
	:m_index(0), m_time(0)
{
}

InterviewUI::~InterviewUI()
{
	delete m_motion;
}

void InterviewUI::Init()
{
}

void InterviewUI::FixedUpdate()
{
}

void InterviewUI::Update(float deltaTime)
{
	m_time += deltaTime;

	//시간 조건 수정 필요
	if (m_time > 1)
	{
		m_index++;

		if (m_index >= m_motion->GetTotalFrame())
		{
			m_index = 0;
		}

		m_time = 0;
	}
}

void InterviewUI::LateUpdate()
{
}

void InterviewUI::PreRender()
{
}

void InterviewUI::Render(DirectY* directY)
{

	if (m_motion->GetTotalFrame() < 2)
	{
		directY->DrawBitmap(m_motion->GetSprite(0));
	}
	else
	{
		directY->DrawBitmap(m_motion->GetSprite(m_index));
	}

}

void InterviewUI::PostRender(DirectY* directY)
{
}

void InterviewUI::Release()
{
}

ElementType InterviewUI::GetElementType() const
{
	return ElementType();
}

GameObject* InterviewUI::GetGameObject() const
{
	return m_gameObject;
}

void InterviewUI::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
