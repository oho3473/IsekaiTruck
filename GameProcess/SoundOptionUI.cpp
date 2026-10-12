#include "SoundOptionUI.h"
#include "Slider.h"
#include "Transform.h"
#include "SoundManager.h"
#include "GameObject.h"

SoundOptionUI::SoundOptionUI(Slider* masterVolumeSlider, Slider* BGMVolumeSlider, Slider* SFXVolumeSlider)
	: m_masterVolumeSlider(masterVolumeSlider),
	m_BGMVolumeSlider(BGMVolumeSlider),
	m_SFXVolumeSlider(SFXVolumeSlider),
	m_myTransform(nullptr),
	m_volumeRatio(0)
{
	
}

SoundOptionUI::~SoundOptionUI()
{
	delete UISprite;
	delete m_bgmTiltle;
	delete m_masterTiltle;
	delete m_sfxTiltle;
}

void SoundOptionUI::Init()
{
	m_myTransform = ((Transform*)m_gameObject->GetElement(ElementType::Transform));

	// 슬라이더들의 초기화를 하자
	m_masterVolumeSlider->Init();
	m_BGMVolumeSlider->Init();
	m_SFXVolumeSlider->Init();

	m_gameObject->SetIsDraw(false);
	//m_gameObject->SetActive(m_gameObject->GetActive());

	m_masterTiltle->m_drawPos = { 45, 90 };
	m_bgmTiltle->m_drawPos = { 45, 190 };
	m_sfxTiltle->m_drawPos = { 45, 290 };

}

void SoundOptionUI::FixedUpdate()
{

}

void SoundOptionUI::Update(float deltaTime)
{

	//m_myTransform->SetLocalPosition({ 100,100 });

	SoundManager::GetInstance().MasterGroupVolume(m_masterVolumeSlider->m_areaRatio);
	SoundManager::GetInstance().ChannelGroupVolume(ChannalGroups::BGM, m_BGMVolumeSlider->m_areaRatio);
	SoundManager::GetInstance().ChannelGroupVolume(ChannalGroups::SFX, m_SFXVolumeSlider->m_areaRatio);
	SoundManager::GetInstance().ChannelGroupVolume(ChannalGroups::UI, m_SFXVolumeSlider->m_areaRatio);

}

void SoundOptionUI::LateUpdate()
{

}

void SoundOptionUI::PreRender()
{

}

void SoundOptionUI::Render(DirectY* directY)
{
	directY->DrawBitmap(UISprite);
	directY->DrawBitmap(m_masterTiltle);
	directY->DrawBitmap(m_sfxTiltle);
	directY->DrawBitmap(m_bgmTiltle);
}

void SoundOptionUI::PostRender(DirectY* directY)
{
	
}

void SoundOptionUI::Release()
{

}

ElementType SoundOptionUI::GetElementType() const
{
	return ElementType::Script;
}

GameObject* SoundOptionUI::GetGameObject() const
{
	return m_gameObject;
}

void SoundOptionUI::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void SoundOptionUI::SetVolumeRatio(float ratio)
{
	SoundManager::GetInstance().MasterGroupVolume(ratio);
}

void SoundOptionUI::SetDraw()
{
	m_gameObject->SetIsDraw(false);
	m_gameObject->SetActive(false);
}
