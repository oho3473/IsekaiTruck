#pragma once
#include <ElementBase.h>
#include "DirectY.h"

class Slider;
class Transform;
class GameObject;

class OptionUI : public ElementBase
{
public:
	OptionUI(Slider* masterVolumeSlider, Slider* BGMVolumeSlider, Slider* SFXVolumeSlider);
	~OptionUI();

	virtual void Init() override;

	virtual void FixedUpdate() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate() override;

	virtual void PreRender() override;
	virtual void Render(DirectY* directY) override;
	virtual void PostRender() override;

	virtual void Release() override;

	virtual ElementType GetElementType() const override;

	virtual GameObject* GetGameObject() const override;
	virtual void SetGameObject(GameObject* gameObject) override;
 /*---------------------------------------------------------------*/
	void SetVolumeRatio(float ratio);

	Graphics::Sprite* GetUISprite() const { return UISprite; }
	void SetUISprite(Graphics::Sprite* val) { UISprite = val; }
	void SetDraw();

private:
	Slider* m_masterVolumeSlider;
	Slider* m_BGMVolumeSlider;
	Slider* m_SFXVolumeSlider;

	Graphics::Sprite* UISprite;

	Transform* m_myTransform;

	float m_volumeRatio;

	

};

