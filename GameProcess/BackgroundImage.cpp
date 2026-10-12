#include "BackgroundImage.h"
#include "DirectY.h"

BackgroundImage::BackgroundImage(Graphics::Sprite* image)
	:m_image(image)
{

}

BackgroundImage::~BackgroundImage()
{
	delete m_image;
}

void BackgroundImage::Init()
{

}

void BackgroundImage::FixedUpdate()
{

}

void BackgroundImage::Update(float deltaTime)
{

}

void BackgroundImage::LateUpdate()
{

}

void BackgroundImage::PreRender()
{

}

void BackgroundImage::Render(DirectY* directY)
{
	directY->DrawBitmap(m_image);
}

void BackgroundImage::PostRender(DirectY* directY)
{

}

void BackgroundImage::Release()
{

}

ElementType BackgroundImage::GetElementType() const
{
	return ElementType::Script;
}

GameObject* BackgroundImage::GetGameObject() const
{
	return m_gameObject;
}

void BackgroundImage::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
