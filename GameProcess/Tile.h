#pragma once
#include <ElementBase.h>
#include "../DirectY/DirectY.h"
struct Graphics::Sprite;

class Tile : public ElementBase
{
public:
	Tile();
	virtual ~Tile();
	
public:
	/*------------------------------------------------------------*/
	virtual void Init() override;

	virtual void FixedUpdate() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate() override;

	virtual void PreRender() override;
	virtual void Render(DirectY* directY) override;
	virtual void PostRender(DirectY* directY) override;

	virtual void Release() override;

	virtual ElementType GetElementType() const override;
	virtual GameObject* GetGameObject() const override;
	virtual void SetGameObject(GameObject* gameObject) override;
	/*------------------------------------------------------------*/

	Graphics::Sprite* GetSprite() const { return m_sprite; }
	void SetSprite(Graphics::Sprite* val) { m_sprite = val; }

	void SetDrawPos(float x, float y) { m_posX = x; m_posY = y; }
private:
	Graphics::Sprite* m_sprite;
	float m_posX, m_posY;
};

