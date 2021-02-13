#pragma once
#include "Singleton.h"

namespace ngenius
{
	class TimeSettings final : public Singleton<TimeSettings>
	{
	public:
		float GetElapsed() const;

		void Update();
		bool CatchUp();
	
	private:
		friend class Singleton<TimeSettings>;
		explicit TimeSettings();

		const bool USEFIXFRAMETIME = true;
		const int FRAMETIME = 16;
		const int MAXFRAMETIME = 100;

		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using Clock = std::chrono::high_resolution_clock;

		TimePoint m_LastTimePoint;
		float m_ElapsedTime;
		float m_FixedElapsedTime;
		float m_Lag;
	};
}

