#include "CloudObject.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Transform.h"
#include "Sprite.h"

CloudObject::CloudObject(GameEngine* gameEngine, DirectY* directY, CloudObject::State state)
{
	m_gameEngine = gameEngine;
	m_directY = directY;
	m_currentImage = state;
	switch (m_currentImage)
	{
		case State::cloud1_1:
			cloud1_1 = m_directY->ReadSpriteTxtFile("cloud288216", m_gameEngine->GetImage("Cloud11"));
			break;
		case State::cloud1_2:
			cloud1_2 = m_directY->ReadSpriteTxtFile("cloud288216", m_gameEngine->GetImage("Cloud12"));
			break;
		case State::cloud2_1:
			cloud2_1 = m_directY->ReadSpriteTxtFile("cloud288144", m_gameEngine->GetImage("Cloud21"));
			break;
		case State::cloud2_2:
			cloud2_2 = m_directY->ReadSpriteTxtFile("cloud288144", m_gameEngine->GetImage("Cloud22"));
			break;
		case State::cloud3_1:
			cloud3_1 = m_directY->ReadSpriteTxtFile("cloud288180", m_gameEngine->GetImage("Cloud31"));
			break;
		case State::cloud3_2:
			cloud3_2 = m_directY->ReadSpriteTxtFile("cloud288180", m_gameEngine->GetImage("Cloud32"));
			break;
		case State::cloud4_1:
			cloud4_1 = m_directY->ReadSpriteTxtFile("cloud288144", m_gameEngine->GetImage("Cloud41"));
			break;
		case State::cloud4_2:
			cloud4_2 = m_directY->ReadSpriteTxtFile("cloud288144", m_gameEngine->GetImage("Cloud42"));
			break;
		case State::cloud5_1:
			cloud5_1 = m_directY->ReadSpriteTxtFile("cloud288216", m_gameEngine->GetImage("Cloud51"));
			break;
		case State::cloud5_2:
			cloud5_2 = m_directY->ReadSpriteTxtFile("cloud288216", m_gameEngine->GetImage("Cloud52"));
			break;
		case State::cloud6_1:
			cloud6_1 = m_directY->ReadSpriteTxtFile("cloud360252", m_gameEngine->GetImage("Cloud61"));
			break;
		case State::cloud6_2:
			cloud6_2 = m_directY->ReadSpriteTxtFile("cloud360252", m_gameEngine->GetImage("Cloud62"));
			break;
		default:
			break;
	}

}

CloudObject::~CloudObject()
{
	switch (m_currentImage)
	{
		case State::cloud1_1:
			delete cloud1_1;
			break;
		case State::cloud1_2:
			delete cloud1_2;
			break;
		case State::cloud2_1:
			delete cloud2_1;
			break;
		case State::cloud2_2:
			delete cloud2_2;
			break;
		case State::cloud3_1:
			delete cloud3_1;
			break;
		case State::cloud3_2:
			delete cloud3_2;
			break;
		case State::cloud4_1:
			delete cloud4_1;
			break;
		case State::cloud4_2:
			delete cloud4_2;
			break;
		case State::cloud5_1:
			delete cloud5_1;
			break;
		case State::cloud5_2:
			delete cloud5_2;
			break;
		case State::cloud6_1:
			delete cloud6_1;
			break;
		case State::cloud6_2:
			delete cloud6_2;
			break;
		default:
			break;
	}
}

void CloudObject::Init()
{

}

void CloudObject::FixedUpdate()
{

}

void CloudObject::Update(float deltaTime)
{

}

void CloudObject::LateUpdate()
{

}

void CloudObject::PreRender()
{

}

void CloudObject::Render(DirectY* directY)
{
	switch (m_currentImage)
	{
		case State::cloud1_1:
			directY->DrawBitmap(cloud1_1);
			break;
		case State::cloud1_2:
			directY->DrawBitmap(cloud1_2);
			break;
		case State::cloud2_1:
			directY->DrawBitmap(cloud2_1);
			break;
		case State::cloud2_2:
			directY->DrawBitmap(cloud2_2);
			break;
		case State::cloud3_1:
			directY->DrawBitmap(cloud3_1);
			break;
		case State::cloud3_2:
			directY->DrawBitmap(cloud3_2);
			break;
		case State::cloud4_1:
			directY->DrawBitmap(cloud4_1);
			break;
		case State::cloud4_2:
			directY->DrawBitmap(cloud4_2);
			break;
		case State::cloud5_1:
			directY->DrawBitmap(cloud5_1);
			break;
		case State::cloud5_2:
			directY->DrawBitmap(cloud5_2);
			break;
		case State::cloud6_1:
			directY->DrawBitmap(cloud6_1);
			break;
		case State::cloud6_2:
			directY->DrawBitmap(cloud6_2);
			break;
		default:
			break;

	}
}

void CloudObject::PostRender(DirectY* directY)
{

}

void CloudObject::Release()
{

}

ElementType CloudObject::GetElementType() const
{
	return ElementType::Script;
}

GameObject* CloudObject::GetGameObject() const
{
	return m_gameObject;
}

void CloudObject::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
