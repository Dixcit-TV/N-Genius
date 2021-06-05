#include "GameModeManager.h"

ngenius::GameMode* ngenius::GameModeManager::m_CurrentGameMode = new GameMode();

void ngenius::GameModeManager::Update()
{
	m_CurrentGameMode->Update();
}

void ngenius::GameModeManager::SetGameMode(GameMode* pNewGameMode)
{
	delete m_CurrentGameMode;
	m_CurrentGameMode = pNewGameMode;
}
