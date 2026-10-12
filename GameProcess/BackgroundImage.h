#pragma once
#include "ElementBase.h"

namespace Graphics { struct Sprite; }

class BackgroundImage : public ElementBase
{
public:
	BackgroundImage(Graphics::Sprite* image);
	virtual ~BackgroundImage();

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

	void SetImage(Graphics::Sprite* val) { m_image = val; }

protected:
	Graphics::Sprite* m_image;
};

