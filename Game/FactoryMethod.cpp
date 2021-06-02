#include "FactoryMethod.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Commands.h"
#include "EnemyController.h"
#include "InputManager.h"
#include "Pyramid.h"
#include "CharacterController.h"
#include "EnemySpawner.h"
#include "LifeComponent.h"
#include "Score.h"
#include "TextComponent.h"
#include "TextureComponent.h"

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateQbert(const ngenius::Transform& transform
	, std::shared_ptr<Pyramid> pyramidComp
	, std::shared_ptr<ngenius::TextComponent> scoreTextComp
	, const std::string& name, PlayerTag tag)
{
	CellFace face{ CellFace::TOP };
	
	auto qbertGO = std::make_shared<ngenius::GameObject>(transform, name);
	qbertGO->GetTransform().SetScale(2.f, 2.f);
	auto qbertComp = qbertGO->AddComponent<CharacterController>(10.f, face);

	auto a = ngenius::Make_Delegate(std::weak_ptr(pyramidComp), &Pyramid::UpdateCell);
	qbertComp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbertGO->AddComponent<ngenius::TextureComponent>("Sprites/Character.png", glm::vec2(0.5f, 1.f));
	auto scoreComp = qbertGO->AddComponent<Score>();
	scoreComp->RegisterScoreUpdateEvent("ScoreTextUpdateEvent", Make_Delegate(std::weak_ptr(scoreTextComp), &ngenius::TextComponent::SetText));
	auto lifeComp = qbertGO->AddComponent<LifeComponent>(3);
	//lifeComp->RegisterUpdateEvent("ScoreTextUpdateEvent", std::bind(&ngenius::TextComponent::SetText, scoreTextComp, std::placeholders::_1));
	qbertComp->RegisterJumpOutEvent("JumpOutEvent", Make_Delegate(std::weak_ptr(lifeComp), &LifeComponent::ApplyDamage));

	pyramidComp->RegisterColorChangeEvent("UpdateScore" + name + "Event", Make_Delegate(std::weak_ptr(scoreComp), &Score::UpdateScore));

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

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateSlickSam(const ngenius::Transform& transform, std::shared_ptr<Pyramid> pyramidComp, std::shared_ptr<EnemySpawner> spawner, EnemyType type)
{
	std::string name{ type == EnemyType::SAM ? "Sam" : "Slick" };
	auto slickSamGO = std::make_shared<ngenius::GameObject>(transform, name);
	slickSamGO->GetTransform().SetScale(2.f, 2.f);
	auto enemyController = slickSamGO->AddComponent<EnemyController>(5.f, CellFace::TOP);
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(slickSamGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(slickSamGO, Direction::SOUTH_WEST));
	enemyController->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, true));
	auto lifeComp = slickSamGO->AddComponent<LifeComponent>(1);
	lifeComp->RegisterHealthDepletedEvent("RespawnEvent", std::bind(&EnemySpawner::QueueSpawn, spawner, type == EnemyType::SAM ? EnemyType::SLICK : EnemyType::SAM));
	lifeComp->RegisterHealthDepletedEvent("DeathEvent", Make_Delegate(std::weak_ptr(slickSamGO), &ngenius::GameObject::Delete));
	enemyController->RegisterJumpOutEvent("JumpOutEvent", Make_Delegate(std::weak_ptr(lifeComp), &LifeComponent::ApplyDamage));
	
	slickSamGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return slickSamGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateEgg(const ngenius::Transform& transform, std::shared_ptr<EnemySpawner> spawner, EnemyType)
{
	std::string name{ "Egg" };
	auto eggGO = std::make_shared<ngenius::GameObject>(transform, name);
	eggGO->GetTransform().SetScale(2.f, 2.f);
	auto enemyController = eggGO->AddComponent<EnemyController>(5.f, CellFace::TOP);
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(eggGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(eggGO, Direction::SOUTH_WEST));
	auto lifeComp = eggGO->AddComponent<LifeComponent>(1);

	//Temp
	lifeComp->RegisterHealthDepletedEvent("RespawnEvent", std::bind(&EnemySpawner::QueueSpawn, spawner, EnemyType::EGG));


	
	lifeComp->RegisterHealthDepletedEvent("DeathEvent", Make_Delegate(std::weak_ptr(eggGO), &ngenius::GameObject::Delete));
	enemyController->RegisterJumpOutEvent("JumpOutEvent", Make_Delegate(std::weak_ptr(lifeComp), &LifeComponent::ApplyDamage));
	
	eggGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return eggGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateCoily(const ngenius::Transform& transform, std::shared_ptr<EnemySpawner> spawner, EnemyType)
{
	std::string name{ "Coily" };
	auto coilyGO = std::make_shared<ngenius::GameObject>(transform, name);
	coilyGO->GetTransform().SetScale(2.f, 2.f);
	auto enemyController = coilyGO->AddComponent<EnemyController>(5.f, CellFace::TOP);
	enemyController->RegisterMoveCommand(Direction::NORTH_EAST, new MoveCommand(coilyGO, Direction::NORTH_EAST));
	enemyController->RegisterMoveCommand(Direction::NORTH_WEST, new MoveCommand(coilyGO, Direction::NORTH_WEST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(coilyGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(coilyGO, Direction::SOUTH_WEST));
	auto lifeComp = coilyGO->AddComponent<LifeComponent>(1);
	lifeComp->RegisterHealthDepletedEvent("RespawnEvent", std::bind(&EnemySpawner::QueueSpawn, spawner, EnemyType::EGG));
	lifeComp->RegisterHealthDepletedEvent("DeathEvent", Make_Delegate(std::weak_ptr(coilyGO), &ngenius::GameObject::Delete));
	enemyController->RegisterJumpOutEvent("JumpOutEvent", Make_Delegate(std::weak_ptr(lifeComp), &LifeComponent::ApplyDamage));
	
	coilyGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", glm::vec2(0.5f, 1.f));

	return coilyGO;
}

std::shared_ptr<ngenius::GameObject> FactoryMethod::CreateUggWrongWay(const ngenius::Transform& transform, std::shared_ptr<EnemySpawner> spawner, EnemyType type)
{
	std::string name{ type == EnemyType::UGG ? "Ugg" : "WrongWay" };
	CellFace face{ type == EnemyType::UGG ? CellFace::RIGHT : CellFace::LEFT };
	glm::vec2 texPivot{ type == EnemyType::UGG ? glm::vec2{0.f, 0.5f} : glm::vec2{1.f, 0.5f} };
	auto UggWrongWayGO = std::make_shared<ngenius::GameObject>(transform, name);
	UggWrongWayGO->GetTransform().SetScale(2.f, 2.f);
	auto enemyController = UggWrongWayGO->AddComponent<EnemyController>(5.f, face);
	enemyController->RegisterMoveCommand(Direction::SOUTH_EAST, new MoveCommand(UggWrongWayGO, Direction::SOUTH_EAST));
	enemyController->RegisterMoveCommand(Direction::SOUTH_WEST, new MoveCommand(UggWrongWayGO, Direction::SOUTH_WEST));
	auto lifeComp = UggWrongWayGO->AddComponent<LifeComponent>(1);

	int r{ Helpers::RandValue(0, 1) };
	lifeComp->RegisterHealthDepletedEvent("RespawnEvent", std::bind(&EnemySpawner::QueueSpawn, spawner, r == 0 ? EnemyType::UGG : EnemyType::WRONGWAY));
	lifeComp->RegisterHealthDepletedEvent("DeathEvent", Make_Delegate(std::weak_ptr(UggWrongWayGO), &ngenius::GameObject::Delete));
	enemyController->RegisterJumpOutEvent("JumpOutEvent", Make_Delegate(std::weak_ptr(lifeComp), &LifeComponent::ApplyDamage));
	
	UggWrongWayGO->AddComponent<ngenius::TextureComponent>("Sprites/" + name + ".png", texPivot);

	return UggWrongWayGO;
}