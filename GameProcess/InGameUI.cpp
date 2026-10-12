#include "InGameUI.h"


#include "GameObject.h"
#include "Transform.h"

#include "InputManager.h"
//
//#include "MathUtils.h"
//#include "Constants.h"
//#include <cmath>

//temp
#include "Sprite.h"
#include "DirectY.h"

#include <iostream>

InGameUI::InGameUI()
{
}

InGameUI::~InGameUI()
{
	delete m_sprite;
}

void InGameUI::Init()
{
}

void InGameUI::FixedUpdate()
{
}

void InGameUI::Update(float deltaTime)
{
}

void InGameUI::LateUpdate()
{
}

void InGameUI::PreRender()
{
}

void InGameUI::Render(DirectY* directY)
{
	((Transform*)(m_gameObject->GetElement(ElementType::Transform)))->SetLocalScale({ 1, 1 });
	directY->DrawBitmap(m_sprite);
}

void InGameUI::PostRender(DirectY* directY)
{
}

void InGameUI::Release()
{
}

ElementType InGameUI::GetElementType() const
{
	return ElementType::Script;
}

GameObject* InGameUI::GetGameObject() const
{
	return m_gameObject;
}

void InGameUI::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
