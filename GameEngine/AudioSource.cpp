#include "AudioSource.h"

AudioSource::AudioSource(GameEngine* gameEngine)
	: m_gameEngine(gameEngine), m_sounds{}
{
	m_soundManager = &SoundManager::GetInstance();
}

AudioSource::~AudioSource()
{

}

void AudioSource::Init()
{

}

void AudioSource::FixedUpdate()
{

}

void AudioSource::Update(float deltaTime)
{

}

void AudioSource::LateUpdate()
{

}

void AudioSource::PreRender()
{

}

void AudioSource::Render(DirectY* directY)
{

}

void AudioSource::PostRender(DirectY* directY)
{

}

void AudioSource::Release()
{

}

ElementType AudioSource::GetElementType() const
{
	return ElementType::AudioSource;
}

GameObject* AudioSource::GetGameObject() const
{
	return m_gameObject;
}

void AudioSource::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

/*-----------------------------------------------------------------------------*/
/// <summary>
/// 파일 재생
/// </summary>
/// <param name="soundName">파일명</param>
void AudioSource::PlayBack(std::string soundName, float volume, FMOD_MODE mode)
{
	m_soundManager->PlayBack(m_sounds[soundName], volume, PlayType::SFX, mode );
}

void AudioSource::PlayBackVector(int index)
{
	m_soundManager->PlayBack(m_soundVector[index]);
}

/// <summary>
/// 반복재생
/// </summary>
/// <param name="soundName">파일명</param>
void AudioSource::LoopPlayBack(std::string soundName)
{
	m_soundManager->PlayBack(m_sounds[soundName], 1.f, PlayType::BGM);
}
/// <summary>
/// 특정 사운드 멈춤
/// </summary>
/// <param name="soundName">파일명</param>
void AudioSource::StopSound(std::string soundName)
{
	m_soundManager->StopSound(m_sounds[soundName]);
}

void AudioSource::StopChannel(ChannalGroups channelgroup)
{
	m_soundManager->StopChannelGroup(channelgroup);
}

/// <summary>
/// 특정사운드 일시정지
/// </summary>
/// <param name="soundName">파일명</param>
void AudioSource::PauseSound(std::string soundName)
{
	m_soundManager->PauseSound(m_sounds[soundName]);
}

/// <summary>
/// 특정사운드 일시정지 해제
/// </summary>
/// <param name="soundName">파일명</param>
void AudioSource::PauseSoundPlay(std::string soundName)
{
	m_soundManager->PauseSoundPlay(m_sounds[soundName]);
}

/// <summary>
/// 페이드인
/// </summary>
/// <param name="soundName">파일명</param>
/// <param name="speed">재생속도 기본은 1.f</param>
void AudioSource::PitchShift(std::string soundName, float speed)
{
	m_soundManager->SetPitch(m_sounds[soundName], speed);
}
/// <summary>
/// 페이드인
/// </summary>
/// <param name="soundName">파일명</param>
/// <param name="length">페이드인 시간</param>
void AudioSource::FadeIn(std::string soundName, int length)
{
	m_soundManager->FadeIn(m_sounds[soundName], length);
}
/// <summary>
/// 페이드아웃
/// </summary>
/// <param name="soundName">파일명</param>
/// <param name="length">페이드아웃 시간</param>
void AudioSource::FadeOut(std::string soundName, int length)
{
	m_soundManager->FadeOut(m_sounds[soundName], length);
}


bool AudioSource::isPlaying(std::string soundName)
{
	return 	m_soundManager->IsPlaying(m_sounds[soundName]);
}

bool AudioSource::isPlaying(int index)
{
	return 	m_soundManager->IsPlaying(m_soundVector[index]);
}

/// <summary>
/// 오디오소스벡터 에 파일 추가
/// </summary>
/// <param name="soundName">파일명</param>
void AudioSource::AddSound(std::string soundName)
{
	m_sounds[soundName] = m_gameEngine->GetSound(soundName);
}

void AudioSource::AddSoundVector(std::string soundName)
{
	m_soundVector.push_back(m_gameEngine->GetSound(soundName));
}
