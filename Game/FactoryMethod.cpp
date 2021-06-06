#include "FactoryMethod.h"

#define WIN32_LEAN_AND_MEAN
#include <SDL_mixer.h>
#include <windows.h>

#include "Button.h"
#include "Commands.h"
#include "EnemyController.h"
#include "InputManager.h"
#include "Pyramid.h"
#include "CharacterController.h"
#include "EnemySpawner.h"
#include "LifeComponent.h"
#include "Score.h"
#include "ServiceLocator.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "Font.h"
#include "GameModeManager.h"
#include "LifeDisplay.h"
#include "QbertGameMode.h"
#include "SceneManager.h"

#pragma region GameObjects
std::shared_ptr<ngenius::GameObject> FactoryMethod::CreatePyramid()
{
	QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
	const GameData& gameData{ pGameMode->GetGameData() };
	const LevelData& level{ gameData.pStageData->levels[gameData.currentLevelIdx] };
	
	auto pyramid = std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(250.f, 600.f)), "Pyramid");
	auto pyramidComp = pyramid->AddComponent<Pyramid>(level.rowCount, 40.f, level.ColorSchemeIndex, level.colorRverting, level.hasIntermediateColor);
	pyramidComp->RegisterCompletionEvent("ColorCompletionEvent", []()
	{
		SavePlayerData();
		QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
		pGameMode->SetState(GameState::SWITCH_LEVEL);
	});
	pyramid->AddComponent<EnemySpawner>();
	return pyramid;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateQbert(const ngenius::Transform& transform
	, std::shared_ptr<Pyramid> pyramidComp
	, std::shared_ptr<ngenius::TextComponent> scoreTextComp
	, PlayerTag tag)
{
	CellFace face{ CellFace::TOP };

	QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
	const GameData& gameData{ pGameMode->GetGameData() };

	std::string name{ tag == PlayerTag::PLAYER1 ? "Player_1" : "Player_2" };
	const PlayerData& data{ gameData.playerData.find(name)->second };
	const int health{ data.health };
	const int score{ data.score };
	const bool isDead{ data.isDead };
	
	auto qbertGO = std::make_shared<ngenius::GameObject>(transform, name, "Player");
	qbertGO->GetTransform().SetScale(2.f, 2.f);
	qbertGO->SetEnable(!isDead);
	auto qbertComp = qbertGO->AddComponent<CharacterController>(10.f, face);

	auto a = Make_Delegate(std::weak_ptr(pyramidComp), &Pyramid::UpdateCell);
	qbertComp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbertGO->AddComponent<ngenius::TextureComponent>("Sprites/Character.png", glm::vec2(0.5f, 1.f));
	
	auto scoreComp = qbertGO->AddComponent<Score>();
	scoreComp->RegisterScoreUpdateEvent("ScoreTextUpdateEvent", Make_Delegate(std::weak_ptr(scoreTextComp), &ngenius::TextComponent::SetText));
	scoreComp->SetScore(score);
	
	auto lifeComp = qbertGO->AddComponent<LifeComponent>(health);
	lifeComp->RegisterHealthDepletedEvent("DeathEvent", &DeathEventCheck);
	qbertComp->RegisterJumpOutEvent("JumpOutLifeLostEvent", std::bind(&LifeComponent::ApplyDamage, lifeComp, 1));
	
	auto rigidBodyComp = qbertGO->AddComponent<ngenius::RigidBody>(10.f, 10.f, glm::vec2(0.f, -20.f));
	rigidBodyComp->RegisterOnOverlapEvent("OverlapEnemyEvent", &OverlapResponsePlayerEnemy);
	rigidBodyComp->RegisterOnOverlapEvent("OverlapSlickSamEvent", &OverlapResponsePlayerSlickSam);

	const glm::vec2& startPos{ transform.GetPosition() };
	qbertComp->RegisterJumpOutEvent("JumpOutPositionEvent", [&transform = qbertGO->GetTransform(), startPos](){ transform.SetPosition(startPos); });

	pyramidComp->RegisterColorChangeEvent("UpdateScore" + name + "Event", Make_Delegate(std::weak_ptr(scoreComp), &Score::UpdateScore));

	InitPlayerInput(qbertGO, tag);
	
	return qbertGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateSlickSam(std::shared_ptr<Pyramid> pyramidComp, std::shared_ptr<EnemySpawner> spawner, EnemyType type)
{
	std::string name{ type == EnemyType::SAM ? "Sam" : "Slick" };
	glm::vec2 startPosition{ pyramidComp->GetTopPosition(CellFace::TOP) };
	
	auto slickSamGO = std::make_shared<ngenius::GameObject>(ngenius::Transform(startPosition, glm::vec2(2.f, 2.f)), name, "SlickSam");
	auto enemyController = slickSamGO->AddComponent<EnemyController>(5.f, CellFace::TOP);
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(slickSamGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(slickSamGO, Direction::SOUTH_WEST));
	enemyController->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, true));
	auto lifeComp = slickSamGO->AddComponent<LifeComponent>(1);
	lifeComp->RegisterHealthDepletedEvent("RespawnEvent", std::bind(&EnemySpawner::QueueSpawn, spawner, type == EnemyType::SAM ? EnemyType::SLICK : EnemyType::SAM, startPosition, false));
	lifeComp->RegisterHealthDepletedEvent("DeathEvent", Make_Delegate(std::weak_ptr(slickSamGO), &ngenius::GameObject::Delete));
	enemyController->RegisterJumpOutEvent("JumpOutEvent", std::bind(&LifeComponent::ApplyDamage, lifeComp, 1));
	auto rigidBodyComp = slickSamGO->AddComponent<ngenius::RigidBody>(10.f, 10.f, glm::vec2(0.f, -20.f));
	
	slickSamGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return slickSamGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateEgg(std::shared_ptr<Pyramid> pyramidComp, std::shared_ptr<EnemySpawner> spawner, EnemyType)
{
	std::string name{ "Egg" };
	glm::vec2 startPosition{ pyramidComp->GetTopPosition(CellFace::TOP) };
	
	auto eggGO = std::make_shared<ngenius::GameObject>(ngenius::Transform(startPosition, glm::vec2(2.f, 2.f)), name, "Enemy");
	auto enemyController = eggGO->AddComponent<EnemyController>(5.f, CellFace::TOP);
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(eggGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(eggGO, Direction::SOUTH_WEST));
	enemyController->RegisterEndMoveEvent("TransformEvent", [pyramidComp, spawner, go = std::weak_ptr(eggGO)](const glm::vec2& thisPosition)
	{
		int r{};
		int c{};
		pyramidComp->GetRowAndColumnFromPosition(thisPosition, r, c);
		if (r == 0)
		{
			spawner->QueueSpawn(EnemyType::COILY, thisPosition, true);
			go.lock()->Delete();
		}
	});
	auto lifeComp = eggGO->AddComponent<LifeComponent>(1);

	//Temp
	lifeComp->RegisterHealthDepletedEvent("RespawnEvent", std::bind(&EnemySpawner::QueueSpawn, spawner, EnemyType::EGG, startPosition, false));

	lifeComp->RegisterHealthDepletedEvent("DeathEvent", Make_Delegate(std::weak_ptr(eggGO), &ngenius::GameObject::Delete));
	enemyController->RegisterJumpOutEvent("JumpOutEvent", std::bind(&LifeComponent::ApplyDamage, lifeComp, 1));
	auto rigidBodyComp = eggGO->AddComponent<ngenius::RigidBody>(10.f, 10.f, glm::vec2(0.f, -20.f));
	
	eggGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return eggGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateCoily(const glm::vec2& position, std::shared_ptr<Pyramid> pyramidComp, std::shared_ptr<EnemySpawner> spawner, EnemyType)
{
	std::string name{ "Coily" };
	CellFace face{ CellFace::TOP };
	QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
	auto coilyGO = std::make_shared<ngenius::GameObject>(ngenius::Transform(position, glm::vec2(2.f, 2.f)), name, "Enemy");
	auto lifeComp = coilyGO->AddComponent<LifeComponent>(1);
	lifeComp->RegisterHealthDepletedEvent("RespawnEvent", std::bind(&EnemySpawner::QueueSpawn, spawner, EnemyType::EGG, pyramidComp->GetTopPosition(CellFace::TOP), false));
	lifeComp->RegisterHealthDepletedEvent("DeathEvent", Make_Delegate(std::weak_ptr(coilyGO), &ngenius::GameObject::Delete));

	switch(pGameMode->GetGameType())
	{
	case GameType::VERSUS:
		{
			auto qbertComp = coilyGO->AddComponent<CharacterController>(10.f, face);
			InitPlayerInput(coilyGO, PlayerTag::PLAYER2);
			qbertComp->RegisterJumpOutEvent("JumpOutEvent", std::bind(&LifeComponent::ApplyDamage, lifeComp, 1));
			lifeComp->RegisterHealthDepletedEvent("UnbindInputEvent", []()
			{
				ngenius::InputManager& inputManager{ ngenius::InputManager::GetInstance() };
				inputManager.RemoveInputBinding("Move2_NortWest");
				inputManager.RemoveInputBinding("Move2_SouthWest");
				inputManager.RemoveInputBinding("Move2_SouthEast");
				inputManager.RemoveInputBinding("Move2_NortEast");
			});
		}
		break;
	default:
		{
			auto enemyController = coilyGO->AddComponent<EnemyController>(5.f, face);
			enemyController->RegisterMoveCommand(Direction::NORTH_EAST, new MoveCommand(coilyGO, Direction::NORTH_EAST));
			enemyController->RegisterMoveCommand(Direction::NORTH_WEST, new MoveCommand(coilyGO, Direction::NORTH_WEST));
			enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(coilyGO, Direction::SOUTH_EAST));
			enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(coilyGO, Direction::SOUTH_WEST));
			enemyController->RegisterJumpOutEvent("JumpOutEvent", std::bind(&LifeComponent::ApplyDamage, lifeComp, 1));
		}
		break;
	}

	auto rigidBodyComp = coilyGO->AddComponent<ngenius::RigidBody>(10.f, 10.f, glm::vec2(0.f, -20.f));
	
	coilyGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return coilyGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateUggWrongWay(std::shared_ptr<Pyramid> pyramidComp, std::shared_ptr<EnemySpawner> spawner, EnemyType type)
{
	std::string name{ type == EnemyType::UGG ? "Ugg" : "WrongWay" };
	CellFace face{ type == EnemyType::UGG ? CellFace::RIGHT : CellFace::LEFT };
	glm::vec2 texPivot{ type == EnemyType::UGG ? glm::vec2{0.f, 0.5f} : glm::vec2{1.f, 0.5f} };
	glm::vec2 startPosition{ type == EnemyType::UGG ? pyramidComp->GetBottomRightPosition(CellFace::RIGHT) : pyramidComp->GetBottomLeftPosition(CellFace::LEFT) };
	
	auto UggWrongWayGO = std::make_shared<ngenius::GameObject>(ngenius::Transform(startPosition, glm::vec2(2.f, 2.f)), name, "Enemy");
	auto enemyController = UggWrongWayGO->AddComponent<EnemyController>(5.f, face);
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(UggWrongWayGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(UggWrongWayGO, Direction::SOUTH_WEST));
	auto lifeComp = UggWrongWayGO->AddComponent<LifeComponent>(1);

	int r{ Helpers::RandValue(0, 1) };
	EnemyType spawnType{ r == 0 ? EnemyType::UGG : EnemyType::WRONGWAY };
	glm::vec2 spawnPos{ spawnType == EnemyType::UGG ? pyramidComp->GetBottomRightPosition(CellFace::RIGHT) : pyramidComp->GetBottomLeftPosition(CellFace::LEFT) };
	
	lifeComp->RegisterHealthDepletedEvent("RespawnEvent", std::bind(&EnemySpawner::QueueSpawn, spawner, r == 0 ? EnemyType::UGG : EnemyType::WRONGWAY, spawnPos, false));
	lifeComp->RegisterHealthDepletedEvent("DeathEvent", Make_Delegate(std::weak_ptr(UggWrongWayGO), &ngenius::GameObject::Delete));
	enemyController->RegisterJumpOutEvent("JumpOutEvent", std::bind(&LifeComponent::ApplyDamage, lifeComp, 1));

	glm::vec2 rbCenter{ type == EnemyType::UGG ? glm::vec2{20.f, 0.f} : glm::vec2{-20.f, 0.f} };
	auto rigidBodyComp = UggWrongWayGO->AddComponent<ngenius::RigidBody>(10.f, 10.f, rbCenter);
	
	UggWrongWayGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", texPivot);

	return UggWrongWayGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateButton(const std::string& text, const glm::vec2& position, const std::string& name, int textSize)
{
	auto pfont = ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Font>("Lingua.otf", textSize);
	auto buttonGO{ std::make_shared<ngenius::GameObject>(ngenius::Transform(position), name) };
	auto buttonComp{ buttonGO->AddComponent<ngenius::Button>(text, pfont, SDL_Color{ 255, 255, 255 }, glm::vec2{0.5f, 0.5f}) };
	buttonComp->RegisterOnClickEvent("ButtonClickEvent", []() { ServiceLocator::GetSoundService()->PlaySound("../Data/Sounds/MenuClick.mp3", MIX_MAX_VOLUME); });
	buttonComp->RegisterOnSelectedEvent("ButtonSelectedEvent", []() { ServiceLocator::GetSoundService()->PlaySound("../Data/Sounds/MenuHover.mp3", MIX_MAX_VOLUME); });

	return buttonGO;
}
#pragma endregion

#pragma region CollisionResponses
void FactoryMethod::OverlapResponsePlayerEnemy(std::shared_ptr<ngenius::RigidBody> thisBody, std::shared_ptr<ngenius::RigidBody> overlappingBody)
{
	auto thisGo{ thisBody->GetGameObject() };
	auto collidingGo{ overlappingBody->GetGameObject() };
	if (thisGo->GetTag() == "Player"
		&& collidingGo->GetTag() == "Enemy")
	{
		auto thisLifeComp{ thisGo->GetComponent<LifeComponent>() };
		if (thisLifeComp)
			thisLifeComp->ApplyDamage(1);

		auto collidingLifeComp{ collidingGo->GetComponent<LifeComponent>() };
		if (collidingLifeComp)
			collidingLifeComp->ApplyDamage(1);
	}
}

void FactoryMethod::OverlapResponsePlayerSlickSam(std::shared_ptr<ngenius::RigidBody> thisBody, std::shared_ptr<ngenius::RigidBody> overlappingBody)
{
	auto thisGo{ thisBody->GetGameObject() };
	auto collidingGo{ overlappingBody->GetGameObject() };
	if (thisGo->GetTag() == "Player"
		&& collidingGo->GetTag() == "SlickSam")
	{
		auto thisLifeComp{ thisGo->GetComponent<Score>() };
		if (thisLifeComp)
			thisLifeComp->UpdateScore(ScoreEventType::SLICK_SAM_CAUGHT);

		auto collidingLifeComp{ collidingGo->GetComponent<LifeComponent>() };
		if (collidingLifeComp)
			collidingLifeComp->ApplyDamage(1);
	}
}
#pragma endregion

#pragma region Scenes
void FactoryMethod::InitMainMenu(std::shared_ptr<ngenius::Scene> pScene)
{
	auto backgroundGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2{960.f, 0.f})) };
	backgroundGo->AddComponent<ngenius::TextureComponent>("Main_Background.png", glm::vec2{1.f, 0.f});
	pScene->Add(backgroundGo);
	
	auto singleButton{ CreateButton("Start Single", glm::vec2{ 150.f, 250.f}) };
	singleButton->GetComponent<ngenius::Button>()->RegisterOnClickEvent("GameStartEvent", []()
		{
			QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
			pGameMode->SetGameType(GameType::SINGLE);
			pGameMode->SetState(GameState::GAME_START);
		});
	
	auto coopButton{ CreateButton("Start Co-op", glm::vec2{ 150.f, 350.f}) };
	coopButton->GetComponent<ngenius::Button>()->RegisterOnClickEvent("GameStartEvent", []()
		{
			QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
			pGameMode->SetGameType(GameType::COOP);
			pGameMode->SetState(GameState::GAME_START);
		});
	
	auto versusButton{ CreateButton("Start Versus", glm::vec2{ 150.f, 450.f}) };
	versusButton->GetComponent<ngenius::Button>()->RegisterOnClickEvent("GameStartEvent", []()
		{
			QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
			pGameMode->SetGameType(GameType::VERSUS);
			pGameMode->SetState(GameState::GAME_START);
		});

	auto quitButton{ CreateButton("Quit Game", glm::vec2{ 150.f, 550.f}) };
	quitButton->GetComponent<ngenius::Button>()->RegisterOnClickEvent("QuitEvent", &ngenius::InputManager::PostQuitEvent);

	pScene->Add(singleButton);
	pScene->Add(coopButton);
	pScene->Add(versusButton);
	pScene->Add(quitButton);
}

void FactoryMethod::InitSinglePlayerScene(std::shared_ptr<ngenius::Scene> pScene)
{
	auto pyramid{ CreatePyramid() };
	pScene->Add(pyramid);
	auto pyramidComp{ pyramid->GetComponent<Pyramid>() };

	auto pfont = ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Font>("Lingua.otf", 20);
	auto scoreTextGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 20.f)), "Score_UI_P1") };
	auto textComp = scoreTextGo->AddComponent<ngenius::TextComponent>("Score: 0", pfont);
	pScene->Add(scoreTextGo);

	auto qbert1 = CreateQbert(ngenius::Transform(pyramidComp->GetTopPosition())
		, pyramidComp
		, textComp
		, PlayerTag::PLAYER1);
	auto lifeComp{ qbert1->GetComponent<LifeComponent>() };

	auto LifeDisplaytGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 60.f)), "Life_UI_P1") };
	auto lifeDisplayComp = LifeDisplaytGo->AddComponent<LifeDisplay>(lifeComp->GetHealth());
	lifeComp->RegisterUpdateEvent("UpdateDisplayEvent", Make_Delegate(std::weak_ptr(lifeDisplayComp), &LifeDisplay::UpdateLifeDrawCount));
	
	pScene->Add(LifeDisplaytGo);

	pScene->Add(qbert1);
}

void FactoryMethod::InitCoopScene(std::shared_ptr<ngenius::Scene> pScene)
{
	auto pyramid{ CreatePyramid() };
	pScene->Add(pyramid);
	auto pyramidComp{ pyramid->GetComponent<Pyramid>() };

	auto pfont = ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Font>("Lingua.otf", 20);
	auto scoreTextGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 20.f)), "Score_UI_P1") };
	auto textComp = scoreTextGo->AddComponent<ngenius::TextComponent>("Score: 0", pfont);
	pScene->Add(scoreTextGo);

	auto qbert1 = CreateQbert(ngenius::Transform(pyramidComp->GetBottomLeftPosition())
		, pyramidComp
		, textComp
		, PlayerTag::PLAYER1);
	auto lifeComp{ qbert1->GetComponent<LifeComponent>() };

	auto LifeDisplaytGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 60.f)), "Life_UI_P1") };
	auto lifeDisplayComp = LifeDisplaytGo->AddComponent<LifeDisplay>(lifeComp->GetHealth());
	lifeComp->RegisterUpdateEvent("UpdateDisplayEvent", Make_Delegate(std::weak_ptr(lifeDisplayComp), &LifeDisplay::UpdateLifeDrawCount));
	pScene->Add(LifeDisplaytGo);

	pScene->Add(qbert1);

	auto scoreText2Go{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(940.f, 20.f)), "Score_UI_P2") };
	auto textComp2 = scoreText2Go->AddComponent<ngenius::TextComponent>("Score: 0", pfont, SDL_Color{ 255, 255, 255 }, glm::vec2{ 1.f, 0.f });
	pScene->Add(scoreText2Go);

	auto qbert2 = CreateQbert(ngenius::Transform(pyramidComp->GetBottomRightPosition())
		, pyramidComp
		, textComp2
		, PlayerTag::PLAYER2);
	auto lifeComp2{ qbert2->GetComponent<LifeComponent>() };

	auto LifeDisplayt2Go{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(890.f, 60.f)), "Life_UI_P1") };
	auto lifeDisplay2Comp = LifeDisplayt2Go->AddComponent<LifeDisplay>(lifeComp2->GetHealth());

	qbert2->GetComponent<LifeComponent>()->RegisterUpdateEvent("UpdateDisplayEvent", Make_Delegate(std::weak_ptr(lifeDisplay2Comp), &LifeDisplay::UpdateLifeDrawCount));
	pScene->Add(LifeDisplayt2Go);

	pScene->Add(qbert2);
}

void FactoryMethod::InitVersusScene(std::shared_ptr<ngenius::Scene> pScene)
{
	auto pyramid{ CreatePyramid() };
	pScene->Add(pyramid);
	auto pyramidComp{ pyramid->GetComponent<Pyramid>() };

	auto pfont = ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Font>("Lingua.otf", 20);
	auto scoreTextGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 20.f)), "Score_UI_P1") };
	auto textComp = scoreTextGo->AddComponent<ngenius::TextComponent>("Score: 0", pfont);
	pScene->Add(scoreTextGo);

	auto qbert1 = CreateQbert(ngenius::Transform(pyramidComp->GetTopPosition())
		, pyramidComp
		, textComp
		, PlayerTag::PLAYER1);
	
	auto lifeComp{ qbert1->GetComponent<LifeComponent>() };

	auto LifeDisplaytGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 60.f)), "Life_UI_P1") };
	auto lifeDisplayComp = LifeDisplaytGo->AddComponent<LifeDisplay>(lifeComp->GetHealth());
	lifeComp->RegisterUpdateEvent("UpdateDisplayEvent", Make_Delegate(std::weak_ptr(lifeDisplayComp), &LifeDisplay::UpdateLifeDrawCount));
	
	pScene->Add(LifeDisplaytGo);

	pScene->Add(qbert1);
}

void FactoryMethod::InitGameOverScene(std::shared_ptr<ngenius::Scene> pScene)
{
	auto backgroundGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2{480.f, 300.f})) };
	backgroundGo->AddComponent<ngenius::TextureComponent>("game-over.jpg", glm::vec2{ 0.5f, 0.5f });
	pScene->Add(backgroundGo);
	
	auto backToMainButton{ CreateButton("Back To Main Menu", glm::vec2{ 300.f, 650.f}) };
	backToMainButton->GetComponent<ngenius::Button>()->RegisterOnClickEvent("BackToMainEvent", []()
		{
			QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
			pGameMode->SetState(GameState::MAIN_MENU);
		});

	auto quitButton{ CreateButton("Quit Game", glm::vec2{ 700.f, 650.f}) };
	quitButton->GetComponent<ngenius::Button>()->RegisterOnClickEvent("QuitEvent", &ngenius::InputManager::PostQuitEvent);

	pScene->Add(backToMainButton);
	pScene->Add(quitButton);
}
#pragma endregion

#pragma region Misc
void FactoryMethod::DeathEventCheck()
{
	auto players{ ngenius::SceneManager::GetInstance().GetCurrentScene()->GetAllGameObjectsWithTag("Player") };
	bool anyAlive{ std::any_of(std::begin(players), std::end(players), [](const std::shared_ptr<ngenius::GameObject>& pPlayer)
	{
		auto lifeComp{ pPlayer->GetComponent<LifeComponent>() };
		return lifeComp && lifeComp->GetHealth() > 0;
	}) };

	if (!anyAlive)
	{
		QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
		pGameMode->SetState(GameState::GAME_OVER);
	}
}

void FactoryMethod::SavePlayerData()
{
	QbertGameMode* pGameMode{ dynamic_cast<QbertGameMode*>(ngenius::GameModeManager::GetGameMode()) };
	GameData& gameData{ pGameMode->GetGameData() };
	auto players{ ngenius::SceneManager::GetInstance().GetCurrentScene()->GetAllGameObjectsWithTag("Player") };

	for (const auto& pPlayer : players)
	{
		PlayerData& data{ gameData.playerData[pPlayer->GetName()] };
		data.health = pPlayer->GetComponent<LifeComponent>()->GetHealth();
		data.score = pPlayer->GetComponent<Score>()->GetScore();
		data.isDead = !pPlayer->IsEnabled();
	}
}

void FactoryMethod::InitPlayerInput(std::shared_ptr<ngenius::GameObject> go, PlayerTag tag)
{
	ngenius::InputManager& inputManager{ ngenius::InputManager::GetInstance() };
	switch (tag)
	{
	case PlayerTag::PLAYER1:
	{
		const int gamepadId{ inputManager.RegisterGamepad() };
		inputManager.BindInput("Move_NortWest", new MoveCommand(go, Direction::NORTH_WEST),
			{ ngenius::Input('W', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
				, ngenius::Input(XINPUT_GAMEPAD_DPAD_UP, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

		inputManager.BindInput("Move_SouthWest", new MoveCommand(go, Direction::SOUTH_WEST),
			{ ngenius::Input('A', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
				, ngenius::Input(XINPUT_GAMEPAD_DPAD_LEFT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

		inputManager.BindInput("Move_SouthEast", new MoveCommand(go, Direction::SOUTH_EAST),
			{ ngenius::Input('S', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
				, ngenius::Input(XINPUT_GAMEPAD_DPAD_DOWN, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

		inputManager.BindInput("Move_NortEast", new MoveCommand(go, Direction::NORTH_EAST),
			{ ngenius::Input('D', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
				, ngenius::Input(XINPUT_GAMEPAD_DPAD_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });
	}
	break;
	case PlayerTag::PLAYER2:
	{
		const int gamepadId = inputManager.RegisterGamepad();
		inputManager.BindInput("Move2_NortWest", new MoveCommand(go, Direction::NORTH_WEST),
			{ ngenius::Input(VK_UP, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
				, ngenius::Input(XINPUT_GAMEPAD_DPAD_UP, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

		inputManager.BindInput("Move2_SouthWest", new MoveCommand(go, Direction::SOUTH_WEST),
			{ ngenius::Input(VK_LEFT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
				, ngenius::Input(XINPUT_GAMEPAD_DPAD_LEFT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

		inputManager.BindInput("Move2_SouthEast", new MoveCommand(go, Direction::SOUTH_EAST),
			{ ngenius::Input(VK_DOWN, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
				, ngenius::Input(XINPUT_GAMEPAD_DPAD_DOWN, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

		inputManager.BindInput("Move2_NortEast", new MoveCommand(go, Direction::NORTH_EAST),
			{ ngenius::Input(VK_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
				, ngenius::Input(XINPUT_GAMEPAD_DPAD_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });
	}
	break;
	}
}
#pragma endregion