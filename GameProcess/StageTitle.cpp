#include "StageTitle.h"
#include "DirectY.h"

#include "Vector2.h"
#include "InputManager.h"

StageTitle::StageTitle(Graphics::Sprite* title1, Graphics::Sprite* title2, Graphics::Sprite* title3)
	:m_titleIndex(0),
	m_title1(title1), m_title2(title2), m_title3(title3)
{

}

StageTitle::~StageTitle()
{
	delete m_title1;
	delete m_title2;
	delete m_title3;
}

void StageTitle::Init()
{

}

void StageTitle::FixedUpdate()
{

}

void StageTitle::Update(float deltaTime)
{
	//60, 214, 631, 1115
	// 676, 214, 676 + 571, 1115
	// 1292, 214, 1292 + 571, 1115
	// 마우스 포인터 위치 받아와서 index 변경
	MathEngine::Vector2 mousePosition = InputManager::GetInstance().GetMousePosition();
	if (mousePosition.y > 214 && mousePosition.y < 1115)
	{
		if (mousePosition.x > 60 && mousePosition.x < 631)
		{
			m_titleIndex = 0;
		}
		else if (mousePosition.x > 676 && mousePosition.x < 676 + 571)
		{
			m_titleIndex = 1;
		}
		else if (mousePosition.x > 1292 && mousePosition.x < 1292 + 571)
		{
			m_titleIndex = 2;
		}
	}
}

void StageTitle::LateUpdate()
{

}

void StageTitle::PreRender()
{

}

void StageTitle::Render(DirectY* directY)
{
	if (m_titleIndex == 0)
	{
		directY->DrawBitmap(m_title1);
	}
	if (m_titleIndex == 1)
	{
		directY->DrawBitmap(m_title2);
	}
	if (m_titleIndex == 2)
	{
		directY->DrawBitmap(m_title3);
	}
}

void StageTitle::PostRender(DirectY* directY)
{

}

void StageTitle::Release()
{

}

ElementType StageTitle::GetElementType() const
{
	return ElementType::Script;
}

GameObject* StageTitle::GetGameObject() const
{
	return m_gameObject;
}

void StageTitle::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
