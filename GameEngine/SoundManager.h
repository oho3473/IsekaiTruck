#pragma once
// 싱글톤 템플릿을 사용하기 위한 헤더
#include "Singleton.h"
#include <string>
#include <iostream>
// FMOD 헤더
#include "../core/inc/fmod.hpp"

#include <functional>

#if x86
#pragma comment (lib,"../core/lib/x86/fmod_vc.lib")
#else
#pragma comment (lib,"../core/lib/x6/fmod_vc.lib")
#endif

enum ChannalGroups
{
	MASTER,
	UI,
	BGM,
	Narr,
	SFX,
};

enum class PlayType
{
	UI,
	BGM,
	NARR,
	SFX,
};

enum FADE
{
	NONE,
	FADE_IN,
	FADE_OUT,
};


/// <summary>
/// 사운드를 관리하고 재생한다.
/// </summary>
class SoundManager : public Singleton<SoundManager>
{
public:

	///  필수코드
	void Init();
	void Update();

	/// 세팅용
	void SetChannelGroup();

	/// get
	// 현재 사운드소스를 리턴합니다
	FMOD::Sound* GetCurrentSound() { return m_sound; }

	/// load
	FMOD::Sound* LoadSound(std::string _path);

	/// 재생
	void PlayBack(FMOD::Sound* _sound, float _volume = 1.f, PlayType _type = PlayType::SFX, FMOD_MODE mode = FMOD_LOOP_OFF, int loopCount = 0);

	// 일시정지
	void PauseSound();
	void PauseSound(FMOD::Sound* targetSound);

	// 일시정지 해제
	void PauseSoundPlay();
	void PauseSoundPlay(FMOD::Sound* targetSound);

	// 정지
	void StopSound(/*멈춤 대상*/);
	void StopSound(FMOD::Sound* targetSound);
	void StopChannelGroup(ChannalGroups channelgroup);
	
	// FadeIn/Out
	void FadeIn(FMOD::Sound* targetSound, int fTime);
	void FadeOut(FMOD::Sound* targetSound, int fTime);

	// pitchShift
	void SetBGMPitch(float pitch);
	void SetPitch(FMOD::Sound* targetSound, float speed);

	// 채널그룹볼륨 제어
	void ChannelGroupVolume(ChannalGroups _channelGroup,float _volume);
	float GetChannelGroupVolume(ChannalGroups _channelGroup);
	void MasterGroupVolume(float _volume);

	// 재생중인지 확인한다.
	bool IsPlaying(FMOD::Sound* targetSound);

	// 리버브(플러그인)
	
	/*--------필요한 것-------------*/
	//
	// 리버브(플러그인)
	// 
	// pan, volume, pitch 컨트롤러

	/*--------하고 싶은 거-------------*/
	// 1. 랜덤재생
	// 2. 크로스 페이더
	// 5. EQ변환
	// 

private:
	SoundManager()
		: m_system(nullptr),
		version(0),
		m_sound(nullptr),
		m_result(), m_isPlaying(false),
		ui_index(0), bgm_index(8), narr_index(16), sfx_index(24),
		m_channels{}, m_ChannelGroup{},
		m_reverb(nullptr), tempSound(nullptr)
	{}
	~SoundManager() {}
private:
	FMOD::System* m_system;
	FMOD_RESULT m_result;

	/// 리버브를 만들어볼까? 

	void TestReverb();

	FMOD::Reverb3D* m_reverb;
	FMOD_VECTOR m_rVector = {0.f, 0.f, 0.f};
	float minDistance = 0;
	float maxDistance = 100;
	FMOD_REVERB_PROPERTIES m_properties = FMOD_PRESET_ARENA;

	// 채널
	const int chNum = 64;
	FMOD::Channel* m_channels[64];
	// bgm[8] ui[8] Narr[8] 2sfx[16]

	// 사운드
	FMOD::Sound* m_sound;
	bool m_isPlaying;
	unsigned int version;

	friend class Singleton<SoundManager>;

	/// 공사중
	FMOD::ChannelGroup *m_ChannelGroup[5];

	int ui_index;
	int bgm_index;
	int narr_index;
	int sfx_index;

	/// 임시저장용 변수
	FMOD::Sound* tempSound;

	unsigned long long dspclock = 0;
	int rate = 0;
};