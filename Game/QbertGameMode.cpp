#include "QbertGameMode.h"

#include "FactoryMethod.h"
#include "SceneManager.h"

QbertGameMode::QbertGameMode()
	: GameMode()
	, m_CurrentState()
	, m_SelectedGameType()
{
	auto clearLambda{ [](std::shared_ptr<ngenius::Scene> pScene) { pScene->Clear(); } };
	
	auto pMainMenuScene{ std::make_shared<ngenius::Scene>("MainMenuScene") };
	pMainMenuScene->RegisterActivateEvent("InitEvent", &FactoryMethod::InitMainMenu);
	pMainMenuScene->RegisterDeactivateEvent("DeactivateEvent", clearLambda);

	auto pSinglePlayerScene{ std::make_shared<ngenius::Scene>("SinglePlayerScene") };
	pSinglePlayerScene->RegisterActivateEvent("InitEvent", &FactoryMethod::InitSinglePlayerScene);
	pSinglePlayerScene->RegisterDeactivateEvent("DeactivateEvent", clearLambda);

	auto pCoopScene{ std::make_shared<ngenius::Scene>("CoopScene") };
	pCoopScene->RegisterActivateEvent("InitEvent", &FactoryMethod::InitCoopScene);
	pCoopScene->RegisterDeactivateEvent("DeactivateEvent", clearLambda);

	auto pVersusScene{ std::make_shared<ngenius::Scene>("VersusScene") };
	pVersusScene->RegisterActivateEvent("InitEvent", &FactoryMethod::InitVersusScene);
	pVersusScene->RegisterDeactivateEvent("DeactivateEvent", clearLambda);

	ngenius::SceneManager::GetInstance().AddScene(pMainMenuScene);
	ngenius::SceneManager::GetInstance().AddScene(pSinglePlayerScene);
	ngenius::SceneManager::GetInstance().AddScene(pCoopScene);
	ngenius::SceneManager::GetInstance().AddScene(pVersusScene);
	
	SetState(GameState::MAIN_MENU);
}

void QbertGameMode::Update()
{
	
}

void QbertGameMode::SetState(GameState newState)
{
	m_CurrentState = newState;
	
	switch (m_CurrentState)
	{
	case GameState::MAIN_MENU:
		ngenius::SceneManager::GetInstance().SetCurrentScene("MainMenuScene");
		break;
	case GameState::GAME_START:
		SelectSceneFromGameType();
		m_CurrentState = GameState::IN_GAME;
		break;
	case GameState::IN_GAME:
		break;
	case GameState::GAME_OVER:
		break;
	}
}

void QbertGameMode::SelectSceneFromGameType() const
{
	switch (m_SelectedGameType)
	{
	case GameType::SINGLE:
		ngenius::SceneManager::GetInstance().SetCurrentScene("SinglePlayerScene");
		break;
	case GameType::COOP:
		ngenius::SceneManager::GetInstance().SetCurrentScene("CoopScene");
		break;
	case GameType::VERSUS:
		ngenius::SceneManager::GetInstance().SetCurrentScene("VersusScene");
		break;
	}
}