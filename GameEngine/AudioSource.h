#pragma once
#include "ElementBase.h"
#include "SoundManager.h"
#include "../core/inc/fmod.hpp"

#include "GameEngine.h"

#include <utility>
#include <vector>
#include <string>
#include <unordered_map>

class AudioSource : public ElementBase
{
public:
	AudioSource(GameEngine* gameEngine);
	~AudioSource();

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
	
	// 사운드를 추가한다.
	void AddSound(std::string soundName);
	void AddSoundVector(std::string soundName);

	// 음원을 재생한다.
	void PlayBack(std::string soundName, float volume = 1.f, FMOD_MODE mode = FMOD_LOOP_OFF);
	void PlayBackVector(int index);
	void LoopPlayBack(std::string soundName);
	void StopSound(std::string soundName);
	void StopChannel(ChannalGroups channelgroup);
	void PauseSound(std::string soundName);
	void PauseSoundPlay(std::string soundName);

	// 피치(스피드)
	void PitchShift(std::string soundName, float speed = 0.1f);
	
	// 페이드인 아웃
	void FadeIn(std::string soundName, int length);
	void FadeOut(std::string soundName, int length);

	// 같은 음원이 재생중인지 확인
	bool isPlaying(std::string soundName);
	bool isPlaying(int index);

private:
	SoundManager* m_soundManager;
	GameEngine* m_gameEngine;
	// 사운드 소스를 이름과 함께 저장
	std::unordered_map<std::string, FMOD::Sound*> m_sounds;
	std::vector < FMOD::Sound* > m_soundVector;

	std::pair<std::string, FMOD::Sound*> m_tempSound;



};

