#include "Collider.h"
#include "../DirectY/directY.h"

#include "Transform.h"
#include "GameObject.h"

#include "../GameProcess/DebugManager.h"

Collider::Collider()
	: m_isCollision(false)
{

}

Collider::~Collider()
{
}

void Collider::Init()
{

}

void Collider::FixedUpdate()
{

}

void Collider::Update(float deltaTime)
{
	m_collider->Update();

	MathEngine::Vector2 objectPos;
	objectPos = ((Transform*)(m_gameObject->GetElement(ElementType::Transform)))->GetLocalPosition();
	m_collider->center = objectPos + m_collider->initCenter; 

}

void Collider::LateUpdate()
{

}

void Collider::PreRender()
{

}

void Collider::Render(DirectY* directY)
{
	if (DebugManager::GetInstance().GetIsDebuging())
	{
		if (m_isCollision)
		{
			directY->SetBrushColor(D2D1::ColorF::Orange);
		}
		else
		{
			directY->SetBrushColor(D2D1::ColorF::Green);
		}

		m_collider->DrawCollider(*directY);
	}
}

void Collider::PostRender(DirectY* directY)
{

}

void Collider::Release()
{

}

ElementType Collider::GetElementType() const
{
	return ElementType::Collider;
}

GameObject* Collider::GetGameObject() const
{
	return m_gameObject;
}

void Collider::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
