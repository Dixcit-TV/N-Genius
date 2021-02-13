#include "PCH.h"
#include "TimeSettings.h"

ngenius::TimeSettings::TimeSettings()
	: m_LastTimePoint(Clock::now())
	, m_ElapsedTime(0.f)
	, m_FixedElapsedTime(0.f)
	, m_Lag(0.f)
{}

inline float ngenius::TimeSettings::GetElapsed() const
{
	return m_ElapsedTime;
}

void ngenius::TimeSettings::Update()
{
	const TimePoint currentTime{ Clock::now() };
	m_ElapsedTime = std::chrono::duration<float>(currentTime - m_LastTimePoint).count();
	m_LastTimePoint = currentTime;

	m_FixedElapsedTime = USEFIXFRAMETIME ? float(FRAMETIME) : m_ElapsedTime;
	m_FixedElapsedTime = std::clamp(m_FixedElapsedTime, 0.f, float(MAXFRAMETIME));
	
	m_Lag += m_FixedElapsedTime;
}

bool ngenius::TimeSettings::CatchUp()
{
	const bool shouldCatchup{ m_Lag >= float(m_FixedElapsedTime) };

	if (shouldCatchup)
		m_Lag -= m_FixedElapsedTime;
	
	return shouldCatchup;
}