#include "Timer.h"
#include "SoundManager.h"

#include "ClearManager.h"

// 테스트를 위한 인풋매니저
// 시간을 사용하기 시작하면 필요없어짐
#include "InputManager.h"
#include "Scene.h"

Timer::Timer(GameEngine* gameEngine, DirectY* directY)
	:m_gameEngine(gameEngine), m_directY(directY),
	m_secondHand(nullptr), m_secondHandBitmap(nullptr),
	m_baseTime(180), m_nowTime(m_baseTime), m_sumTime(0),
	m_timerState(false), m_isPlaying(false),m_isPlayingFast(false),
	m_degree(90), m_audioSource(nullptr)
{
	// 비트맵 로딩
	m_secondHandBitmap = m_gameEngine->GetImage("SecondHand");
	m_secondHand = directY->ReadSpriteTxtFile("SecondHand", m_secondHandBitmap);
	m_secondHand->m_drawPos = { -4.5, -65 };


	//((AudioSource*)(m_gameObject->GetElement(ElementType::AudioSource)))->AddSound("bgm_beep_fest");

// 	m_audioSource->AddSound("bgm_beep_fest");
// 	m_audioSource->AddSound("bgm_beep_slow");
}

Timer::~Timer()
{
	delete m_secondHand;
}

void Timer::Init()
{
	/*m_audioSource = ((AudioSource*)(m_gameObject->GetElement(ElementType::AudioSource)));*/
}

void Timer::FixedUpdate()
{

}

void Timer::Update(float deltaTime)
{
	if (ClearManager::GetInstance().CheckClear())
	{
		StopTimer();
		m_audioSource->StopSound("bgm_beep_slow");
	}

	if(IsTimerON())
	{
		m_sumTime += deltaTime;
		if (m_sumTime > 1)
		{
			m_nowTime = m_nowTime - 1;
			m_sumTime = 0;
			m_degree = (m_nowTime - m_baseTime) * (180 / m_baseTime) + 90;

			//타이머가 시간이 흘러버렸다
			if (m_degree <= -90)
			{
				StopTimer();
				ClearManager::GetInstance().SetDead(true);
				m_audioSource->StopSound("bgm_beep_slow");
				
				m_directY->DeletePaticleEffect();
				m_gameEngine->SetNextScene("GameOverScene");
				ClearManager::GetInstance().SetClear(false);
				ClearManager::GetInstance().SetDead(false);
			}
		}
	}
	
	// 타이머에 따라 음악속도증가
	if (GetNowTime() < m_baseTime / 2)
	{
		m_audioSource->PitchShift("bgm_beep_slow", 1.05f);
		if(!m_isPlayingFast)
		{
			m_audioSource->PlayBack("sfx_timer_warning");
			m_isPlayingFast = true;
		}
	}

	if (m_timerState && !m_isPlaying)
	{
		/// audioElement 이식작업중
		m_audioSource->LoopPlayBack("bgm_beep_slow");
		m_isPlaying = true;
	}
}

void Timer::LateUpdate()
{

}

void Timer::PreRender()
{

}

void Timer::Render(DirectY* directY)
{
	// 잘 보이게 글자색변경
	//directY->SetBrushColor(1, 1, 1, 1);
	//directY->DrawText(0, 0, 20.f,TextAlignMent::LEFT, L"Verdana", "time : %f", GetNowTime());
		
	directY->DrawBitmap(m_secondHand, 1.f);
	((Transform*)(m_gameObject->GetElement(ElementType::Transform)))->SetLocalPosition({ (float)1631.7,685 });
	((Transform*)(m_gameObject->GetElement(ElementType::Transform)))->SetLocalRotation((float)-m_degree);
}

void Timer::PostRender(DirectY* directY)
{

}

void Timer::Release()
{

}

ElementType Timer::GetElementType() const
{
	return ElementType::Script;
}

GameObject* Timer::GetGameObject() const
{
	return m_gameObject;
}

void Timer::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}


