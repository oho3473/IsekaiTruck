#pragma once
#include <ElementBase.h>
#include "DirectY.h"

class Slider;
class Transform;
class GameObject;

class SoundOptionUI : public ElementBase
{
public:
	SoundOptionUI(Slider* masterVolumeSlider, Slider* BGMVolumeSlider, Slider* SFXVolumeSlider);
	~SoundOptionUI();

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
 /*---------------------------------------------------------------*/
	void SetVolumeRatio(float ratio);

	Graphics::Sprite* GetUISprite() const { return UISprite; }
	void SetUISprite(Graphics::Sprite* val) { UISprite = val; }
	void SetDraw();

	Graphics::Sprite* GetMasterTiltle() const { return m_masterTiltle; }
	void SetMasterTiltle(Graphics::Sprite* val) { m_masterTiltle = val; }
	Graphics::Sprite* GetBgmTiltle() const { return m_bgmTiltle; }
	void SetBgmTiltle(Graphics::Sprite* val) { m_bgmTiltle = val; }
	Graphics::Sprite* GetSfxTiltle() const { return m_sfxTiltle; }
	void SetSfxTiltle(Graphics::Sprite* val) { m_sfxTiltle = val; }
private:
	Slider* m_masterVolumeSlider;
	Slider* m_BGMVolumeSlider;
	Slider* m_SFXVolumeSlider;

	Graphics::Sprite* UISprite;
	Graphics::Sprite* m_masterTiltle;
	Graphics::Sprite* m_bgmTiltle;
	Graphics::Sprite* m_sfxTiltle;

	Transform* m_myTransform;

	float m_volumeRatio;

	

};

