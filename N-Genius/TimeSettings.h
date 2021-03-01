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
		void TrySleep() const;
		//bool CatchUp();

		float GetElapsed() const { return m_ElapsedTime; }
		unsigned int GetFPS() const { return m_FPS; }
	
	private:
		friend class Singleton<TimeSettings>;
		explicit TimeSettings();

		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using Clock = std::chrono::high_resolution_clock;

		TimePoint m_LastTimePoint;
		
		float m_ElapsedTime;
		float m_Lag;
		float m_FPSTimerCount;
		unsigned int m_FPS;
		unsigned int m_FPSCounter;

		const float FIXEDDELTATIME = 0.2f;
		const float MAXFRAMETIME = 0.1f;
		const int FRAMETIMEMS = 16;
		const bool USEFIXEDFRAMETIME = true;
	};
}

