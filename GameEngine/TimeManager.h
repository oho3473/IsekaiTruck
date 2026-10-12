#pragma once
#include <windows.h>

// #include <chrono> 시간 관련 라이브러리 시간 날때 확인해볼것

/// <summary>
/// DeltaTime을 측정하기 위한 클래스
/// 
/// 2023.07.21 김유진
/// </summary>
class TimeManager
{
public:
	static float m_timeScale;

	TimeManager();
	~TimeManager();

	void	Init();
	void	Update();

	int		GetFps()		const { return (int)(m_fps * m_timeScale); }
	float	GetDeltaTime()	const { return m_deltaTime * m_timeScale; }

private:
	float	m_deltaTime;			// 1 프레임 당 걸리는 시간 
	int		m_fps;					// 초당 프레임 수

	LARGE_INTEGER m_previousCount;	// 이전 frequency 동안 몇 번 진동했는지 저장하기 위한 변수
	LARGE_INTEGER m_frequency;		// 1초에 몇 번 진동 했는지 저장하기 위한 변수

	int		m_accumulationCount;	// 1초 동안 몇 프레임이 실행되었는지 세기 위한 변수
	float	m_accumulationTime;		// 1초를 세기 위한 변수
};

