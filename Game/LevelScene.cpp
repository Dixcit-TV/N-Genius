#include "LevelScene.h"
#include <GameObject.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Commands.h"
#include "InputManager.h"
#include "Pyramid.h"
#include "Qbert.h"
#include "ResourceManager.h"
#include "Stats.h"
#include "TextComponent.h"
#include "TextureComponent.h"

void LevelScene::Initialise()
{
	auto pyramid = std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(200.f, 520.f)), "Pyramid");
	auto pyramidComp = pyramid->AddComponent<Pyramid>(7, 40.f, false, false);
	Add(pyramid);

	auto pfont = ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Font>("Lingua.otf", 20);
	auto scoreTextGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 20.f)), "Score_UI") };
	auto textComp = scoreTextGo->AddComponent<ngenius::TextComponent>("Score: 0", pfont);
	Add(scoreTextGo);

	auto qbertGO = std::make_shared<ngenius::GameObject>(ngenius::Transform(pyramidComp->GetTopPosition(), glm::vec2(2.f, 2.f)), "Player1");
	auto qbertComp = qbertGO->AddComponent<Qbert>(10.f);
	qbertComp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbertGO->AddComponent<ngenius::TextureComponent>("Sprites/Character.png", glm::vec2(0.5f, 0.4f));
	auto statComp = qbertGO->AddComponent<Stats>(3);
	statComp->RegisterScoreUpdateEvent("ScoreTextUpdateEvent", std::bind(&ngenius::TextComponent::SetText, textComp, std::placeholders::_1));
	Add(qbertGO);

	auto qbert2GO = std::make_shared<ngenius::GameObject>(ngenius::Transform(pyramidComp->GetTopPosition(), glm::vec2(2.f, 2.f)), "Player2");
	auto qbert2Comp = qbert2GO->AddComponent<Qbert>(10.f);
	qbert2Comp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbert2GO->AddComponent<ngenius::TextureComponent>("Sprites/Character.png", glm::vec2(0.5f, 0.4f));
	Add(qbert2GO);

	pyramidComp->RegisterColorChangeEvent("UpdateScoreEvent", std::bind(&Stats::UpdateScore, statComp, std::placeholders::_1));

	int gamepadId{ngenius::InputManager::GetInstance().RegisterGamepad() };
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
		{ngenius::Input('D', 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
			, ngenius::Input(XINPUT_GAMEPAD_DPAD_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

	gamepadId = ngenius::InputManager::GetInstance().RegisterGamepad();
	ngenius::InputManager::GetInstance().BindInput("Move2_NortWest", new MoveCommand(qbert2GO, Direction::NORTH_WEST), 
		{ ngenius::Input(VK_UP, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
			, ngenius::Input(XINPUT_GAMEPAD_DPAD_UP, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });
	
	ngenius::InputManager::GetInstance().BindInput("Move2_SouthWest", new MoveCommand(qbert2GO, Direction::SOUTH_WEST), 
		{ ngenius::Input(VK_LEFT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
			, ngenius::Input(XINPUT_GAMEPAD_DPAD_LEFT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });
	
	ngenius::InputManager::GetInstance().BindInput("Move2_SouthEast", new MoveCommand(qbert2GO, Direction::SOUTH_EAST), 
		{ ngenius::Input(VK_DOWN, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
			, ngenius::Input(XINPUT_GAMEPAD_DPAD_DOWN, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });
	
	ngenius::InputManager::GetInstance().BindInput("Move2_NortEast", new MoveCommand(qbert2GO, Direction::NORTH_EAST), 
		{ ngenius::Input(VK_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::KEYBOARD)
			, ngenius::Input(XINPUT_GAMEPAD_DPAD_RIGHT, 0, ngenius::InputType::BUTTON, ngenius::InputSource::GAMEPAD, gamepadId) });

}
