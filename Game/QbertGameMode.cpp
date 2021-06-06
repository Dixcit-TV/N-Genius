#include "QbertGameMode.h"

#include "FactoryMethod.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "SoundEffect.h"

QbertGameMode::QbertGameMode()
	: GameMode()
	, m_GameData()
	, m_CurrentState(GameState::MAIN_MENU)
	, m_SelectedGameType()
	, m_StateChanged(false)
{
	auto clearLambda{ [](std::shared_ptr<ngenius::Scene> pScene)
	{
		ngenius::InputManager::GetInstance().ClearInputBinding();
		pScene->Clear();
	} };
	
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

	auto pEndScene{ std::make_shared<ngenius::Scene>("EndScene") };
	pEndScene->RegisterActivateEvent("InitEvent", &FactoryMethod::InitGameOverScene);
	pEndScene->RegisterDeactivateEvent("DeactivateEvent", clearLambda);

	ngenius::SceneManager::GetInstance().AddScene(pMainMenuScene);
	ngenius::SceneManager::GetInstance().AddScene(pSinglePlayerScene);
	ngenius::SceneManager::GetInstance().AddScene(pCoopScene);
	ngenius::SceneManager::GetInstance().AddScene(pVersusScene);
	ngenius::SceneManager::GetInstance().AddScene(pEndScene);
	ngenius::SceneManager::GetInstance().SetCurrentScene("MainMenuScene");
}

void QbertGameMode::Update()
{
	if (m_StateChanged)
	{
		m_StateChanged = false;
		
		switch (m_CurrentState)
		{
		case GameState::MAIN_MENU:
			SoundEffect::StopAll();
			ngenius::SceneManager::GetInstance().SetCurrentScene("MainMenuScene");
			break;
		case GameState::GAME_START:
			InitGameData();
			SelectSceneFromGameType();
			SetState(GameState::IN_GAME);
			break;
		case GameState::IN_GAME:
			break;
		case GameState::SWITCH_LEVEL:
			{
				SoundEffect::StopAll();
				++m_GameData.currentLevelIdx;
				const int levelCount{ static_cast<int>(m_GameData.pStageData->levels.size()) };
				if (m_GameData.currentLevelIdx >= levelCount)
					SetState(GameState::GAME_OVER);
				else
				{
					ngenius::InputManager::GetInstance().ClearInputBinding();
					ngenius::SceneManager::GetInstance().ReloadCurrentScene();
					SetState(GameState::IN_GAME);
				}
			}
			break;
		case GameState::GAME_OVER:
			SoundEffect::StopAll();
			ServiceLocator::GetSoundService()->PlaySound("../Data/Sounds/tune.mp3", MIX_MAX_VOLUME);
			ngenius::SceneManager::GetInstance().SetCurrentScene("EndScene");
			break;
		}
	}
}

void QbertGameMode::SetState(GameState newState)
{
	m_StateChanged = m_CurrentState != newState;
	m_CurrentState = newState;
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

void QbertGameMode::InitGameData()
{
	const int startHealth{ 3 };
	const int startScore{ 0 };

	m_GameData.playerData.clear();
	m_GameData.playerData["Player_1"] = PlayerData{ "Player_1", startHealth, startScore, false };

	if (m_SelectedGameType == GameType::COOP)
		m_GameData.playerData["Player_2"] = PlayerData{ "Player_2", startHealth, startScore, false };

	m_GameData.pStageData = ngenius::ResourceManager::GetInstance().LoadResource<StageData>("Levels/Stage_1.json");
	m_GameData.currentLevelIdx = 0;
}