#pragma once
#include "ElementBase.h"
#include <functional>

class DirectY;
class GameObject;
class Transform;

namespace Graphics { struct Sprite; }

class Slider : public ElementBase
{
public:
	Slider(int left, int right, int top, int bottom, int controllerX, int controllerY, float ratio);
	~Slider();

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

	void OnChange();

	void SetOnChange(std::function<void(void)> onChange);

	void SetBackground(Graphics::Sprite* background);
	void SetFillArea(Graphics::Sprite* fillArea);
	void SetHandle(Graphics::Sprite* handle);
	void SetDraw();

public:
	int m_backgroundLeft;
	int m_backgroundRight;
	int m_backgroundTop;
	int m_backgroundBottom;

	// 컨트롤러 조작 중인지
	bool m_isControll;

	Graphics::Sprite* m_background;
	Graphics::Sprite* m_fillArea;
	Graphics::Sprite* m_handle;

	float m_areaRatio;

	std::function<void(void)> m_onChange;

private:
	Transform* m_myTransform;
};
