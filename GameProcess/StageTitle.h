#pragma once
#include "ElementBase.h"

namespace Graphics { struct Sprite; }

class StageTitle : public ElementBase
{
public:
	StageTitle(Graphics::Sprite* title1, Graphics::Sprite* title2, Graphics::Sprite* title3);
	virtual ~StageTitle();

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

private:
	int m_titleIndex;

	Graphics::Sprite* m_title1;
	Graphics::Sprite* m_title2;
	Graphics::Sprite* m_title3;
};

