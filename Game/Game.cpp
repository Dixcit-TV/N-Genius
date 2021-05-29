#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <functional>
#include <Memory>
#include <Core.h>
#include <SceneManager.h>
#include <Scene.h>

#include <GameObject.h>
#include <InputManager.h>
#include <ResourceManager.h>
#include <SDL_mixer.h>
#include <TextComponent.h>
#include <TextureComponent.h>

#include "Commands.h"
#include "Pyramid.h"
#include "Qbert.h"
#include "ServiceLocator.h"
#include "Stats.h"

using namespace ngenius;

void PrintInfo();
void LoadGame();

int main(int, char* [])
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cerr << "error when calling SDL_Init: " << SDL_GetError() << std::endl;
	}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cerr << "error when calling Mix_OpenAudio: " << Mix_GetError() << std::endl;
	}

	
	Core core{ "Q-bert", 960, 720 };
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");	
	LoadGame();
    core.Run();

	ServiceLocator::UnRegister();
	Mix_Quit();
    return 0;
}

void LoadGame()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	SceneManager::GetInstance().SetCurrentScene(0);

	auto pyramid = std::make_shared<GameObject>(Transform(glm::vec2(200.f, 520.f)), "Pyramid");
	auto pyramidComp = pyramid->AddComponent<Pyramid>(7, 40.f, false, false);
	scene.Add(pyramid);

	auto pfont = ResourceManager::GetInstance().LoadResource<Font>("Lingua.otf", 20);
	auto scoreTextGo{ std::make_shared<GameObject>(Transform(glm::vec2(20.f, 20.f)), "Score_UI") };
	auto textComp = scoreTextGo->AddComponent<TextComponent>("Score: 0", pfont);
	scene.Add(scoreTextGo);

	auto qbertGO = std::make_shared<GameObject>(Transform(pyramidComp->GetTopPosition(), glm::vec2(2.f, 2.f)), "Player1");
	auto qbertComp = qbertGO->AddComponent<Qbert>(10.f);
	qbertComp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbertGO->AddComponent<TextureComponent>("Sprites/Character.png", glm::vec2(0.5f, 0.4f));
	auto statComp = qbertGO->AddComponent<Stats>(3);
	statComp->RegisterScoreUpdateEvent("ScoreTextUpdateEvent", std::bind(&TextComponent::SetText, textComp, std::placeholders::_1));
	scene.Add(qbertGO);

	auto qbert2GO = std::make_shared<GameObject>(Transform(pyramidComp->GetTopPosition(), glm::vec2(2.f, 2.f)), "Player2");
	auto qbert2Comp = qbert2GO->AddComponent<Qbert>(10.f);
	qbert2Comp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbert2GO->AddComponent<TextureComponent>("Sprites/Character.png", glm::vec2(0.5f, 0.4f));
	scene.Add(qbert2GO);

	pyramidComp->RegisterColorChangeEvent("UpdateScoreEvent", std::bind(&Stats::UpdateScore, statComp, std::placeholders::_1));

	int gamepadId{ InputManager::GetInstance().RegisterGamepad() };
	InputManager::GetInstance().BindInput("Move_NortWest", new MoveCommand(qbertGO, Direction::NORTH_WEST), { Input('W', 0, InputType::BUTTON, InputSource::KEYBOARD), Input(XINPUT_GAMEPAD_DPAD_UP, 0, InputType::BUTTON, InputSource::GAMEPAD, gamepadId) });
	InputManager::GetInstance().BindInput("Move_SouthWest", new MoveCommand(qbertGO, Direction::SOUTH_WEST), { Input('A', 0, InputType::BUTTON, InputSource::KEYBOARD), Input(XINPUT_GAMEPAD_DPAD_LEFT, 0, InputType::BUTTON, InputSource::GAMEPAD, gamepadId) });
	InputManager::GetInstance().BindInput("Move_SouthEast", new MoveCommand(qbertGO, Direction::SOUTH_EAST), { Input('S', 0, InputType::BUTTON, InputSource::KEYBOARD), Input(XINPUT_GAMEPAD_DPAD_DOWN, 0, InputType::BUTTON, InputSource::GAMEPAD, gamepadId) });
	InputManager::GetInstance().BindInput("Move_NortEast", new MoveCommand(qbertGO, Direction::NORTH_EAST), { Input('D', 0, InputType::BUTTON, InputSource::KEYBOARD), Input(XINPUT_GAMEPAD_DPAD_RIGHT, 0, InputType::BUTTON, InputSource::GAMEPAD, gamepadId) });

	gamepadId = InputManager::GetInstance().RegisterGamepad();
	InputManager::GetInstance().BindInput("Move2_NortWest", new MoveCommand(qbert2GO, Direction::NORTH_WEST), { Input(VK_UP, 0, InputType::BUTTON, InputSource::KEYBOARD), Input(XINPUT_GAMEPAD_DPAD_UP, 0, InputType::BUTTON, InputSource::GAMEPAD, gamepadId) });
	InputManager::GetInstance().BindInput("Move2_SouthWest", new MoveCommand(qbert2GO, Direction::SOUTH_WEST), { Input(VK_LEFT, 0, InputType::BUTTON, InputSource::KEYBOARD), Input(XINPUT_GAMEPAD_DPAD_LEFT, 0, InputType::BUTTON, InputSource::GAMEPAD, gamepadId) });
	InputManager::GetInstance().BindInput("Move2_SouthEast", new MoveCommand(qbert2GO, Direction::SOUTH_EAST), { Input(VK_DOWN, 0, InputType::BUTTON, InputSource::KEYBOARD), Input(XINPUT_GAMEPAD_DPAD_DOWN, 0, InputType::BUTTON, InputSource::GAMEPAD, gamepadId) });
	InputManager::GetInstance().BindInput("Move2_NortEast", new MoveCommand(qbert2GO, Direction::NORTH_EAST), { Input(VK_RIGHT, 0, InputType::BUTTON, InputSource::KEYBOARD), Input(XINPUT_GAMEPAD_DPAD_RIGHT, 0, InputType::BUTTON, InputSource::GAMEPAD, gamepadId) });
	
	//PrintInfo();
}

void PrintInfo()
{
	std::cout << "\nPress H: to play Hit sound.\n";
	std::cout << "Press M: to play Menu sound.\n";
	std::cout << "Press A: to switch between SDLAudio Service.\n";
	std::cout << "Press S: to switch between logging Audio Service.\n";
}
