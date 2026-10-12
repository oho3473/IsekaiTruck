#include "Credit.h"
#include "Sprite.h"

#include "DirectY.h"
#include "InputManager.h"

#include "Transform.h"
#include "GameObject.h"

Credit::Credit(Graphics::Sprite* background, Graphics::Sprite* truck, int maxIndex)
	:m_cutScenes(), m_index(0), m_maxIndex(maxIndex)
	, m_background(background), m_truck(truck), m_truckPos(), isFly(false)
{
	
}

Credit::~Credit()
{
	for (int i = 0; i < (int)m_cutScenes.size(); i++)
	{
		delete m_cutScenes[i];
	}
	delete m_background;
	delete m_truck;
}

void Credit::Init()
{

}

void Credit::FixedUpdate()
{

}

void Credit::Update(float deltaTime)
{
	m_time += deltaTime;
	if (m_time > 0.3f)
	{
		if (m_index < m_maxIndex)
		{
			m_index++;
			m_time = 0;
		}
	}
	if (m_index == m_maxIndex)
	{
		m_truckPos += { 3857 / 5 * deltaTime, -2354 / 5 * deltaTime};
	}
}

void Credit::LateUpdate()
{

}

void Credit::PreRender()
{

}

void Credit::Render(DirectY* directY)
{
	Transform* transform = ((Transform*)(m_gameObject->GetElement(ElementType::Transform)));

	directY->DrawBitmap(m_background);

	if (m_index < m_maxIndex)
	{
		directY->DrawBitmap(m_cutScenes[m_index]);
	}
	if (m_index == m_maxIndex)
	{
		if (m_time < 1.f)
		{
			directY->DrawBitmap(m_cutScenes[m_index - 1]);
		}
		else
		{
			if (!isFly)
			{
				m_npcFly();
				isFly = true;
			}
			if (m_time > 6.0f)
			{
				m_goTitle();
			}
		}

		m_truck->m_drawPos = { -3300 + m_truckPos.x, 540 + m_truckPos.y };
		directY->DrawBitmap(m_truck);

		//m_truck->m_drawPos = { -2000 + m_truckPos.x, 2354 + m_truckPos.y };
	}

	transform->SetLocalPosition({ m_truckPos.x - 1800, m_truckPos.y + 1990 });
	transform->SetLocalRotation(-27);
	transform->SetTransform(directY);

	int fontSize = 37;
	directY->SetBrushColor(0, 0, 0, 1);
	directY->DrawText(L"[이세계 용사 메이커]", 800, 0, fontSize, TextAlignMent::LEFT, L"Danjo");
	directY->DrawText(L"이세계에 가고 싶은 자들의 명단", 800, 50, fontSize, TextAlignMent::LEFT, L"Danjo");

	directY->DrawText(L"Programming", 200, 0, fontSize, TextAlignMent::LEFT, L"Danjo");
	directY->DrawText(L"강규석 김유진 김주영", 200, 50, fontSize, TextAlignMent::LEFT, L"Danjo");
	directY->DrawText(L"오해강 유승운 임성균", 200, 100, fontSize, TextAlignMent::LEFT, L"Danjo");

	directY->DrawText(L"Art", -300, 0, fontSize, TextAlignMent::LEFT, L"Danjo");
	directY->DrawText(L"김남진 박한나", -300, 50, fontSize, TextAlignMent::LEFT, L"Danjo");
	directY->DrawText(L"Design", -300, 150, fontSize, TextAlignMent::LEFT, L"Danjo");
	directY->DrawText(L"전상원 진태규", -300, 200, fontSize, TextAlignMent::LEFT, L"Danjo");

	directY->DrawText(L"게임을 즐겨 주셔서 감사합니다.", -1100, 0, fontSize, TextAlignMent::LEFT, L"Danjo");
	directY->DrawText(L"Thanks for Playing!", -1100, 50, fontSize, TextAlignMent::LEFT, L"Danjo");

	transform->SetLocalRotation(0);
	transform->SetLocalPosition({0, 0});
}

/*
[이세계 용사 메이커]

이세계에 가고 싶은 자들의 명단

Programming
강규석
김유진
김주영
오해강
유승운
임성균

Art
김남진
박한나


Design
전상원
진태규


게임을 즐겨 주셔서 감사합니다.
Thanks for Playing.*/

void Credit::PostRender(DirectY* directY)
{

}

void Credit::Release()
{

}

ElementType Credit::GetElementType() const
{
	return ElementType::Script;
}

GameObject* Credit::GetGameObject() const
{
	return m_gameObject;
}

void Credit::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void Credit::AddSprite(Graphics::Sprite* sprite)
{
	m_cutScenes.push_back(sprite);
}
