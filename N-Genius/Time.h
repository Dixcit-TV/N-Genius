#pragma once
#include "Singleton.h"

namespace ngenius
{
	class Time final : public Singleton<Time>
	{
	public:
		float GetElapsed() const;
		int GetFrameTime() const;
	
	private:
		friend class Singleton<Time>;
		explicit Time() = default;

		const int FRAMETIME = 16;
		
		float m_ElapsedTime;
	};
}

