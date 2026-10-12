#include "TimeManager.h"

float TimeManager::m_timeScale = 1.f;

TimeManager::TimeManager()
	:m_deltaTime(0), m_fps(0),
	m_accumulationCount(0), m_accumulationTime(0),
	m_previousCount({}), m_frequency({})
{

}

TimeManager::~TimeManager()
{

}

void TimeManager::Init()
{
	QueryPerformanceCounter(&m_previousCount);
}

void TimeManager::Update()
{
	// 이번 프레임 Count 측정
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);

	// 이번 프레임 빈도 측정
	QueryPerformanceFrequency(&m_frequency);

	m_deltaTime = (currentCount.QuadPart - m_previousCount.QuadPart) / (float)m_frequency.QuadPart;
	m_previousCount = currentCount;
	
	// 1초 동안 몇 번 인지 세기 위해 프레임 수를 1 올리고 시간 누적
	m_accumulationCount++;
	m_accumulationTime += m_deltaTime;

	// 1초가 되면 값 초기화
	if (m_accumulationTime >= 1.f)
	{
		m_fps = (int)(m_accumulationCount / m_accumulationTime);

		m_accumulationTime = 0.f;
		m_accumulationCount = 0;
	}
}
