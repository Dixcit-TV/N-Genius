#include "FactoryMethod.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Commands.h"
#include "EnemyController.h"
#include "InputManager.h"
#include "Pyramid.h"
#include "Qbert.h"
#include "Stats.h"
#include "TextComponent.h"
#include "TextureComponent.h"

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateQbert(const ngenius::Transform& transform
	, std::shared_ptr<Pyramid> pyramidComp
	, std::shared_ptr<ngenius::TextComponent> scoreTextComp
	, const std::string& name, PlayerTag tag)
{
	CellFace face{ CellFace::TOP };
	
	auto qbertGO = std::make_shared<ngenius::GameObject>(transform, name);
	auto qbertComp = qbertGO->AddComponent<Qbert>(10.f, face);
	qbertComp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbertGO->AddComponent<ngenius::TextureComponent>("Sprites/Character.png", glm::vec2(0.5f, 1.f));
	auto statComp = qbertGO->AddComponent<Stats>(3);
	statComp->RegisterScoreUpdateEvent("ScoreTextUpdateEvent", std::bind(&ngenius::TextComponent::SetText, scoreTextComp, std::placeholders::_1));

	pyramidComp->RegisterColorChangeEvent("UpdateScore" + name + "Event", std::bind(&Stats::UpdateScore, statComp, std::placeholders::_1));

	switch(tag)
	{
	case PlayerTag::PLAYER1:
		{
			const int gamepadId{ ngenius::InputManager::GetInstance().RegisterGamepad() };
			ngenius::InputManager::GetInstance().BindInput("Move_NortWest", new MoveCommand(qbertGO, Direction::NORTH_WEST),
				{ ngenius::Input('W', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
					, ngenius::Input(XINPUT_GAMEPAD_DPAD_UP, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

			ngenius::InputManager::GetInstance().BindInput("Move_SouthWest", new MoveCommand(qbertGO, Direction::SOUTH_WEST),
				{ ngenius::Input('A', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
					, ngenius::Input(XINPUT_GAMEPAD_DPAD_LEFT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

			ngenius::InputManager::GetInstance().BindInput("Move_SouthEast", new MoveCommand(qbertGO, Direction::SOUTH_EAST),
				{ ngenius::Input('S', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
					, ngenius::Input(XINPUT_GAMEPAD_DPAD_DOWN, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

			ngenius::InputManager::GetInstance().BindInput("Move_NortEast", new MoveCommand(qbertGO, Direction::NORTH_EAST),
				{ ngenius::Input('D', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
					, ngenius::Input(XINPUT_GAMEPAD_DPAD_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });
		}
		break;
	case PlayerTag::PLAYER2:
		{
			const int gamepadId = ngenius::InputManager::GetInstance().RegisterGamepad();
			ngenius::InputManager::GetInstance().BindInput("Move2_NortWest", new MoveCommand(qbertGO, Direction::NORTH_WEST),
				{ ngenius::Input(VK_UP, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
					, ngenius::Input(XINPUT_GAMEPAD_DPAD_UP, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

			ngenius::InputManager::GetInstance().BindInput("Move2_SouthWest", new MoveCommand(qbertGO, Direction::SOUTH_WEST),
				{ ngenius::Input(VK_LEFT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
					, ngenius::Input(XINPUT_GAMEPAD_DPAD_LEFT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

			ngenius::InputManager::GetInstance().BindInput("Move2_SouthEast", new MoveCommand(qbertGO, Direction::SOUTH_EAST),
				{ ngenius::Input(VK_DOWN, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
					, ngenius::Input(XINPUT_GAMEPAD_DPAD_DOWN, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

			ngenius::InputManager::GetInstance().BindInput("Move2_NortEast", new MoveCommand(qbertGO, Direction::NORTH_EAST),
				{ ngenius::Input(VK_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
					, ngenius::Input(XINPUT_GAMEPAD_DPAD_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });
		}
		break;
	}
	
	return qbertGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateSlickSam(const ngenius::Transform& transform, std::shared_ptr<Pyramid> pyramidComp, EnemyType type)
{
	std::string name{ type == EnemyType::SAM ? "Sam" : "Slick" };
	auto slickSamGO = std::make_shared<ngenius::GameObject>(transform, name);
	auto enemyController = slickSamGO->AddComponent<EnemyController>();
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(slickSamGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(slickSamGO, Direction::SOUTH_WEST));
	
	auto qbertComp = slickSamGO->AddComponent<Qbert>(10.f, CellFace::TOP);
	qbertComp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, true));
	qbertComp->RegisterEndMoveEvent("PickNewMovementEvent", std::bind(&EnemyController::TriggerNextMovement, enemyController, std::placeholders::_1));
	
	slickSamGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return slickSamGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateEgg(const ngenius::Transform& transform, EnemyType)
{
	std::string name{ "Egg" };
	auto eggGO = std::make_shared<ngenius::GameObject>(transform, name);
	auto enemyController = eggGO->AddComponent<EnemyController>();
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(eggGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(eggGO, Direction::SOUTH_WEST));
	
	auto qbertComp = eggGO->AddComponent<Qbert>(10.f, CellFace::TOP);
	qbertComp->RegisterEndMoveEvent("PickNewMovementEvent", std::bind(&EnemyController::TriggerNextMovement, enemyController, std::placeholders::_1));
	eggGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return eggGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateCoily(const ngenius::Transform& transform, EnemyType)
{
	std::string name{ "Coily" };
	auto coilyGO = std::make_shared<ngenius::GameObject>(transform, name);
	auto enemyController = coilyGO->AddComponent<EnemyController>();
	enemyController->RegisterMoveCommand(Direction::NORTH_EAST, new MoveCommand(coilyGO, Direction::NORTH_EAST));
	enemyController->RegisterMoveCommand(Direction::NORTH_WEST, new MoveCommand(coilyGO, Direction::NORTH_WEST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(coilyGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(coilyGO, Direction::SOUTH_WEST));
	
	auto qbertComp = coilyGO->AddComponent<Qbert>(10.f, CellFace::TOP);
	qbertComp->RegisterEndMoveEvent("PickNewMovementEvent", std::bind(&EnemyController::TriggerNextMovement, enemyController, std::placeholders::_1));
	coilyGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return coilyGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateUggWrongWay(const ngenius::Transform& transform, EnemyType type)
{
	std::string name{ type == EnemyType::UGG ? "Ugg" : "WrongWay" };
	CellFace face{ type == EnemyType::UGG ? CellFace::RIGHT : CellFace::LEFT };
	auto UggWrongWayGO = std::make_shared<ngenius::GameObject>(transform, name);
	auto enemyController = UggWrongWayGO->AddComponent<EnemyController>();
	enemyController->RegisterMoveCommand(Direction::NORTH_EAST, new MoveCommand(UggWrongWayGO, Direction::NORTH_EAST));
	enemyController->RegisterMoveCommand(Direction::NORTH_WEST, new MoveCommand(UggWrongWayGO, Direction::NORTH_WEST));
	
	auto qbertComp = UggWrongWayGO->AddComponent<Qbert>(10.f, face);
	qbertComp->RegisterEndMoveEvent("PickNewMovementEvent", std::bind(&EnemyController::TriggerNextMovement, enemyController, std::placeholders::_1));
	
	UggWrongWayGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return UggWrongWayGO;
}