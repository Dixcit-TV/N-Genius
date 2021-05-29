#include "FactoryMethod.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Commands.h"
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
	auto qbertGO = std::make_shared<ngenius::GameObject>(transform, name);
	auto qbertComp = qbertGO->AddComponent<Qbert>(10.f);
	qbertComp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbertGO->AddComponent<ngenius::TextureComponent>("Sprites/Character.png", glm::vec2(0.5f, 0.4f));
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
