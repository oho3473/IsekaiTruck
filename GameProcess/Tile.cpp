#include "Tile.h"
#include "Sprite.h"

Tile::Tile()
	: m_sprite(nullptr)
	, m_posX(0.f)
	, m_posY(0.f)
{

}

Tile::~Tile()
{

}

void Tile::Init()
{

}

void Tile::FixedUpdate()
{

}

void Tile::Update(float deltaTime)
{

}

void Tile::LateUpdate()
{

}

void Tile::PreRender()
{

}

void Tile::Render(DirectY* directY)
{
	// 진짜 멍청한 구조인 것 같긴 함.
	m_sprite->m_drawPos.x = m_posX;
	m_sprite->m_drawPos.y = m_posY;
	directY->DrawBitmap(m_sprite);
}

void Tile::PostRender(DirectY* directY)
{

}

void Tile::Release()
{

}

ElementType Tile::GetElementType() const
{
	return ElementType::Script;
}

GameObject* Tile::GetGameObject() const
{
	return m_gameObject;
}

void Tile::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}
