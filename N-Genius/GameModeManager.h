#pragma once
#include "GameMode.h"
#include "Singleton.h"

namespace ngenius
{
	class GameModeManager final
	{
	public:
		static void Update();
		static void SetGameMode(GameMode*);

		static GameMode* GetGameMode() { return m_CurrentGameMode; }
		static void Destroy() { delete m_CurrentGameMode; }

	private:
		static GameMode* m_CurrentGameMode;
	};
}

