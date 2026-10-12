#include "Camera.h"
#include "InputManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "../DirectY/Sprite.h"
#include "../DirectY/DirectY.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Init()
{

}

void Camera::FixedUpdate()
{

}

void Camera::Update(float deltaTime)
{

}

void Camera::LateUpdate()
{

}

void Camera::PreRender()
{

}

void Camera::Render(DirectY* directY)
{
	Graphics::BaseFigure::Rectangle testRect{ 0,0,100,100 };

	directY->SetBrushColor(0, 0, 0, 1);
	directY->DrawRectangle(testRect);
}

void Camera::PostRender(DirectY* directY)
{

}

void Camera::Release()
{

}

ElementType Camera::GetElementType() const
{
	return ElementType::Script;
}

GameObject* Camera::GetGameObject() const
{
	return m_gameObject;
}

void Camera::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
