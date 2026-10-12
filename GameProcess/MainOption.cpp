#include "MainOption.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "InputManager.h"

MainOption::MainOption()
{
}

MainOption::~MainOption()
{
	delete m_background;
}

void MainOption::Init()
{
	m_gameObject->SetIsDraw(false);
	m_gameObject->SetActive(false);

	m_myTransform = ((Transform*)(m_gameObject->GetElement(ElementType::Transform)));
}

void MainOption::FixedUpdate()
{

}

void MainOption::Update(float deltaTime)
{
}

void MainOption::LateUpdate()
{

}

void MainOption::PreRender()
{

}

void MainOption::Render(DirectY* directY)
{
	directY->DrawBitmap(m_background);
}

void MainOption::PostRender(DirectY* directY)
{

}

void MainOption::Release()
{

}

ElementType MainOption::GetElementType() const
{
	return ElementType::Script;
}

GameObject* MainOption::GetGameObject() const
{
	return m_gameObject;
}

void MainOption::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
