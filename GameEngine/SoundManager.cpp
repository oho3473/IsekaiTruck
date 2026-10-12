#include "SoundManager.h"
#include <windows.h>

/// <summary>
/// FMOD초기화 함수, 시스템을 생성한다.
/// </summary>
void SoundManager::Init()
{
	// System_Create, FMOD 인스턴스 생성

	m_result = FMOD::System_Create(&m_system);
	if (m_result != FMOD_OK) return;
	// init, FMOD초기화
	m_result = m_system->init(64, FMOD_INIT_NORMAL, 0);  // 시스템 초기화 : chaneel 32개까지 사용하겠다.
	if (m_result != FMOD_OK) return;
	// 필요한 초기화는 끝이다

	// 현재 fmod 버전
	m_system->getVersion(&version);
	TestReverb();

	SetChannelGroup();

}

/// <summary>
/// update는 선택사항이 아니다
/// 반드시 일어나야하는 행위이다
/// FMOD 시스템을 업데이트합니다.
/// 이것은 '게임' 틱당 한 번 또는 애플리케이션의 프레임당 한 번 호출되어야 합니다.
/// </summary>
void SoundManager::Update()
{
	m_result = m_system->update();
	if (m_result != FMOD_OK) return;
	// 샘플레이트를 가져온다
	// 타임매니저가 있으면 없어도 괜찮을지도? 
	m_system->getSoftwareFormat(&rate, 0, 0);
}

/// <summary>
/// 사운드파일의 경로를 입력 받고
/// 루프하지 않는 사운드포인터를 반환하는 함수
/// </summary>
/// <param name="soundPath">사운드파일 경로</param>
/// <returns></returns>
FMOD::Sound* SoundManager::LoadSound(std::string _path)
{
	// wav 파일로부터 sound 생성
	m_result = m_system->createSound(/*"jaguar.wav"*/_path.c_str(), FMOD_DEFAULT, 0, &m_sound);
	if (m_result == FMOD_OK)
	{
		return m_sound;
	}
	std::cout << "잘못된 경로입니다" << std::endl;
	return nullptr;
}

/// <summary>
/// 사운드를 재생하는 함수
/// default값은 SFX로 한다.
/// 
/// 모든 종류의 사운드를 재생하는 플레이백 함수
/// 1. 그룹별로 채널을 순회하며 재생한다
/// 2. 채널별로 볼륨 조절이 가능
/// 
/// </summary>
/// <param name="_sound">사운드 객체</param>
/// <param name="_volume">소리 크기</param>
/// <param name="_type">플레이타입
/// </param>
void SoundManager::PlayBack(FMOD::Sound* _sound, float _volume, PlayType _type, FMOD_MODE mode, int loopCount)
{
	switch (_type)
	{
	case (PlayType::UI):
	{
		/// 현재 채널이 재생중인지 확인하고 재생하고 있다면 멈춤
		m_channels[ui_index]->isPlaying(&m_isPlaying);
		if (m_isPlaying)
		{
			m_channels[ui_index]->stop();
		}
		//m_channels[ui_index]->addFadePoint(dspclock, 1.0f);
		// 사운드 모드 설정
		_sound->setMode(mode);
		// 사운드 일시정지
		m_result = m_system->playSound(_sound, m_ChannelGroup[UI], true, &m_channels[ui_index]);
		if (m_result != FMOD_OK) return;
		// 볼륨조절
		m_channels[ui_index]->setVolume(_volume);
		// 사운드 재생
		m_channels[ui_index]->setPaused(false);
		
		///순회재생은 봉인
		ui_index = (ui_index++) % 8;
		break;
	}
	case(PlayType::BGM):
	{

		/// 현재 채널이 재생중인지 확인하고 재생하고 있다면 멈춤
		m_channels[bgm_index]->isPlaying(&m_isPlaying);
		if (m_isPlaying)
		{
			m_channels[bgm_index]->stop();
		}
		//m_channels[bgm_index]->addFadePoint(dspclock, 1.0f);

		// 사운드 모드 설정
		_sound->setMode(FMOD_LOOP_NORMAL);
		// 사운드 일시정지
		m_result = m_system->playSound(_sound, m_ChannelGroup[BGM], true, &m_channels[bgm_index]);
		if (m_result != FMOD_OK) return;
		// 볼륨조절
		m_channels[bgm_index]->setVolume(_volume);
		// 사운드 재생
		m_channels[bgm_index]->setPaused(false);
		
		///순회재생은 봉인
		bgm_index = (bgm_index++) % 8 + 8;
		break;
	}
	case (PlayType::NARR):
	{
		/// 현재 채널이 재생중인지 확인하고 재생하고 있다면 멈춤
		m_channels[narr_index]->isPlaying(&m_isPlaying);
		if (m_isPlaying)
		{
			m_channels[narr_index]->stop();
		}
		//m_channels[narr_index]->addFadePoint(dspclock, 1.0f);
		// 사운드 모드 설정
		_sound->setMode(mode);
		// 사운드 일시정지
		m_result = m_system->playSound(_sound, m_ChannelGroup[Narr], true, &m_channels[narr_index]);
		if (m_result != FMOD_OK) return;
		// 볼륨조절
		m_channels[narr_index]->setVolume(_volume);
		// 사운드 재생
		m_channels[narr_index]->setPaused(false);

		///순회재생은 봉인
		narr_index = (narr_index++) % 8 + 16;
		break;
	}
	case(PlayType::SFX):
	{
		/// 현재 채널이 재생중인지 확인하고 재생하고 있다면 멈춤
		m_channels[sfx_index]->isPlaying(&m_isPlaying);
		if (m_isPlaying)
		{
			m_channels[sfx_index]->stop();
		}
		//m_channels[sfx_index]->addFadePoint(dspclock, 1.0f);

		// 사운드 모드 설정
		_sound->setMode(mode);
		// 사운드 일시정지
		m_result = m_system->playSound(_sound, m_ChannelGroup[SFX], true, &m_channels[sfx_index]);
		if (m_result != FMOD_OK) return;
		// 볼륨조절
		m_channels[sfx_index]->setVolume(_volume);

		m_channels[sfx_index]->setPaused(false);
		m_channels[sfx_index]->setLoopCount(loopCount);
				
		sfx_index = (sfx_index++) % 8 + 24;
		break;
	}
	default:
		break;
	}

}

// 모든채널을 일시정지한다.
// 채널, 채널그룹을 배열로 만들면 반복문으로 가능하지 않을까
void SoundManager::PauseSound()
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->setPaused(true);
	}
}

void SoundManager::PauseSound(FMOD::Sound* targetSound)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->getCurrentSound(&tempSound);
		if (tempSound == targetSound)
		{
			m_channels[i]->setPaused(true);
		}
	}
	tempSound = nullptr;
}


// 모든채널을 일시정지한다.
// 채널, 채널그룹을 배열로 만들면 반복문으로 가능하지 않을까
void SoundManager::PauseSoundPlay(/*일시정지 대상*/)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->setPaused(false);
	}
}


void SoundManager::PauseSoundPlay(FMOD::Sound* targetSound)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->getCurrentSound(&tempSound);
		if (tempSound == targetSound)
		{
			m_channels[i]->setPaused(false);
		}
	}
	tempSound = nullptr;
}

/// <summary>
/// 정지한다.
/// </summary>
void SoundManager::StopSound(/*멈춤 대상*/)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->stop();
	}

}

void SoundManager::StopSound(FMOD::Sound* stopSound)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->getCurrentSound(&tempSound);
		if( tempSound ==  stopSound)
		{
			m_channels[i]->stop();
		}
	}
	tempSound = nullptr;
}

void SoundManager::StopChannelGroup(ChannalGroups channelgroup)
{
	m_ChannelGroup[channelgroup]->stop();
}

void SoundManager::FadeIn(FMOD::Sound* targetSound, int fTime)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->getCurrentSound(&tempSound);
		if (tempSound == targetSound)
		{
			m_channels[i]->getDSPClock(0, &dspclock);
			m_channels[i]->addFadePoint(dspclock, 0.0f);
			m_channels[i]->addFadePoint(dspclock + ((unsigned long long)rate * fTime), 1.0f);
		}
	}
	tempSound = nullptr;
}

void SoundManager::FadeOut(FMOD::Sound* targetSound, int fTime)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->getCurrentSound(&tempSound);
		if (tempSound == targetSound)
		{
			m_channels[i]->getDSPClock(0, &dspclock);		// 현재 dspclock을 가져옴
			m_channels[i]->addFadePoint(dspclock, 1.0f);	// (시간, 볼륨) ,, 설정된 시간에 볼륨을 설정한다.
			m_channels[i]->addFadePoint(dspclock + ((unsigned long long)rate * fTime), 0.0f);    // 
			m_channels[i]->setDelay(0, dspclock + ((unsigned long long)rate * fTime), true);     // 시작시간, 종료시간, true==stop false == pause
		}
	}
	tempSound = nullptr;
}

void SoundManager::SetBGMPitch(float pitch)
{
	for (int i = 8; i < 16; i++)
	{
		m_channels[i]->isPlaying(&m_isPlaying);
		if (m_isPlaying)
		{
			m_channels[i]->setPitch(pitch);

		}
	}
}

void SoundManager::SetPitch(FMOD::Sound* targetSound, float speed)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->getCurrentSound(&tempSound);
		if (tempSound == targetSound)
		{
			m_channels[i]->setPitch(speed);
		}
	}
	tempSound = nullptr;
}

/// <summary>
/// 채널그룹의 볼륨을 조절하는 함수
/// </summary>
/// <param name="_channelGroup">채널을 선택한다.</param>
/// <param name="_volume">볼륨을 조절한다.</param>
void SoundManager::ChannelGroupVolume(ChannalGroups _channelGroup, float _volume)
{
	if (_volume >= 1)
	{
		_volume = 1;
	}
	else if(_volume <= 0)
	{
		_volume = 0;
	}
	m_ChannelGroup[_channelGroup]->setVolume(_volume);
}

float SoundManager::GetChannelGroupVolume(ChannalGroups _channelGroup)
{
	float volume;
	m_ChannelGroup[_channelGroup]->getVolume(&volume);
	return volume;
}

void SoundManager::MasterGroupVolume(float _volume)
{
	m_ChannelGroup[MASTER]->setVolume(_volume);
}

bool SoundManager::IsPlaying(FMOD::Sound* targetSound)
{
	for (int i = 0; i < chNum; i++)
	{
		m_channels[i]->getCurrentSound(&tempSound);
		if (tempSound == targetSound)
		{
			return true;
		}
	}
	return false;
}

void SoundManager::TestReverb()
{
	//1 리버브 객체를 생성한다.
	m_system->createReverb3D(&m_reverb);
	//2 리버브 옵션을 설정한다.
	m_reverb->setProperties(&m_properties);
	//3
	m_reverb->release();
	//m_reverb->set3DAttributes(&m_rVector, minDistance, maxDistance);

}

void SoundManager::SetChannelGroup()
{
	// 마스터 채널 생성
	m_system->getMasterChannelGroup(&m_ChannelGroup[MASTER]);

	// 채널그룹을 생성

	m_system->createChannelGroup("UIGroup", &m_ChannelGroup[UI]);
	m_system->createChannelGroup("BGMGroup", &m_ChannelGroup[BGM]);
	m_system->createChannelGroup("NarrGroup", &m_ChannelGroup[Narr]);
	m_system->createChannelGroup("SFXGroup", &m_ChannelGroup[SFX]);

	// 채널 별로 그룹지정
	int i = 0;
	for (; i < 8; i++)
	{
		m_channels[i]->setChannelGroup(m_ChannelGroup[UI]);
		m_channels[i]->setPriority(0);
	}
	for (; i < 16; i++)
	{
		m_channels[i]->setChannelGroup(m_ChannelGroup[BGM]);
		m_channels[i]->setPriority(10);
	}
	for (; i < 24; i++)
	{
		m_channels[i]->setChannelGroup(m_ChannelGroup[Narr]);
		m_channels[i]->setPriority(20);
	}

	for (; i < 32; i++)
	{
		m_channels[i]->setChannelGroup(m_ChannelGroup[SFX]);
		m_channels[i]->setPriority(30);
	}

}

