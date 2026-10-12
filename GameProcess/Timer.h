#pragma once
#include <ElementBase.h>
#include "../DirectY/DirectY.h"
#include "GameEngine.h"
#include "Transform.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "AudioSource.h"


class GameEngine;
class DirectY;

/// <summary>
/// 타이머 
/// </summary>
class Timer : public ElementBase
{
public:
	Timer(GameEngine* gameEngine, DirectY* directY);
	~Timer();

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

	/*--------------------------------------------------------------------*/
	
	void ResetTimer(float settingTime = 120.f) { m_nowTime = settingTime; }
	void PlusTimer(float plusTime = 0.f) { m_nowTime += plusTime; }
	void MinustTimer(float minusTime = 0.f) { m_nowTime -= minusTime; }

	/*--------------------------------------------------------------------*/
	
	bool IsTimerON() { return m_timerState; }
	bool StartTimer() { m_timerState = true; return m_timerState; }
	bool StopTimer() { m_timerState = false; return m_timerState; }

	/*--------------------------------------------------------------------*/
	float GetNowTime() { return m_nowTime; }

	/*--------------------------------------------------------------------*/
	void SetAudioSource(AudioSource* audioSource) { m_audioSource = audioSource; }

private:
	// 타이머가 갖고있어야 하는것 ?
	// 타이머는 시간을 늘리는게아니라 
	// 주어진 시간에서 차감하는 방식을 사용하는 것이 좋을지도모르겠다. 
	
	// 초침 출력을 위한 비트맵 정보
	Graphics::Sprite* m_secondHand;
	ID2D1Bitmap* m_secondHandBitmap;
	GameEngine* m_gameEngine;
	DirectY* m_directY;

	AudioSource* m_audioSource;

	bool m_timerState;

	bool m_isPlaying;
	bool m_isPlayingFast;

	float m_baseTime;
	float m_nowTime;
	float m_sumTime;

	float m_degree;

};

