#include "PCH.h"
#include "TimeSettings.h"

ngenius::TimeSettings::TimeSettings()
	: m_LastTimePoint(Clock::now())
	, m_ElapsedTime(0.f)
	, m_Lag(0.f)
	, m_FPSTimerCount(0.f)
	, m_FPS(0)
	, m_FPSCounter(0)
{}

void ngenius::TimeSettings::Update()
{
	const TimePoint currentTime{ Clock::now() };
	const float deltaT{ std::chrono::duration<float>(currentTime - m_LastTimePoint).count() };
	m_LastTimePoint = currentTime;
	m_Lag += deltaT;

	m_ElapsedTime = USEFIXEDFRAMETIME ? float(FRAMETIMEMS * 0.001f) : deltaT;
	m_ElapsedTime = std::clamp(m_ElapsedTime, 0.f, float(MAXFRAMETIMEMS * 0.001f));

	m_FPSTimerCount += deltaT;
	++m_FPSCounter;

	if (m_FPSTimerCount >= 1.f)
	{
		m_FPSTimerCount = m_FPSTimerCount - 1.f;
		m_FPS = m_FPSCounter;
		m_FPSCounter = 0;
	}
}

bool ngenius::TimeSettings::CatchUp()
{
	const bool shouldCatchup{ m_Lag >= m_ElapsedTime };

	if (shouldCatchup)
	{
		m_Lag -= m_ElapsedTime;
	}
	
	return shouldCatchup;
}