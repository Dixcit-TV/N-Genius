#include "MiniginPCH.h"
#include "Time.h"

float ngenius::Time::GetElapsed() const
{
	return m_ElapsedTime;
}

int ngenius::Time::GetFrameTime() const
{
	return FRAMETIME;
}