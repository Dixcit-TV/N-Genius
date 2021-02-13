#pragma once
#include "Singleton.h"

namespace ngenius
{
	class TimeSettings final : public Singleton<TimeSettings>
	{
	public:
		~TimeSettings() override = default;
		TimeSettings(const TimeSettings & other) = delete;
		TimeSettings(TimeSettings && other) = delete;
		TimeSettings& operator=(const TimeSettings & other) = delete;
		TimeSettings& operator=(TimeSettings && other) = delete;

		void Update();
		bool CatchUp();

		float GetElapsed() const;
	
	private:
		friend class Singleton<TimeSettings>;
		explicit TimeSettings();

		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using Clock = std::chrono::high_resolution_clock;

		TimePoint m_LastTimePoint;
		
		const bool USEFIXFRAMETIME = true;
		const int FRAMETIME = 16;
		const int MAXFRAMETIME = 100;
		
		float m_ElapsedTime;
		float m_FixedElapsedTime;
		float m_Lag;
	};
}

