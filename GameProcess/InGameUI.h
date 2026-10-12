#pragma once
#include "ElementBase.h"
#include "Vector2.h"

namespace Graphics { struct Sprite; }
namespace Graphics { class Motion; }

class InGameUI : public ElementBase
{
public:
	InGameUI();
	~InGameUI();

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

	//test
	Graphics::Sprite* m_sprite;
};

