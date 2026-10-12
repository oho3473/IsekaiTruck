#pragma once
#include "ElementBase.h"


namespace Graphics { struct Sprite; }
class DirectY;
class GameObject;
class Transform;


class MainOption :
    public ElementBase
{
public:
	MainOption();
	~MainOption();

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

	void SetImage(Graphics::Sprite* sprite) { m_background = sprite; }

private:
	Transform* m_myTransform;
	Graphics::Sprite* m_background;
};

