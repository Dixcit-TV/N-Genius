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

	auto pyramid = std::make_shared<GameObject>(Transform(200.f, 520.f), "Pyramid");
	auto pyramidComp = pyramid->AddComponent<Pyramid>(7, 40.f, false, false);
	scene.Add(pyramid);

	auto qbertGO = std::make_shared<GameObject>(Transform(pyramidComp->GetTopPosition()), "Player1");
	auto qbertComp = qbertGO->AddComponent<Qbert>(10.f);
	qbertComp->RegisterEndMoveEvent("UpdateCellStateEvent", std::bind(&Pyramid::UpdateCell, pyramidComp, std::placeholders::_1, false));
	qbertGO->AddComponent<TextureComponent>("Sprites/Character.png");
	qbertGO->GetTransform().SetPosition(pyramidComp->GetTopPosition());
	scene.Add(qbertGO);
	

	InputManager::GetInstance().BindInput("Move_NortWest", new MoveCommand(qbertGO, Direction::NORTH_WEST), { Input('W', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Move_SouthWest", new MoveCommand(qbertGO, Direction::SOUTH_WEST), { Input('A', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Move_SouthEast", new MoveCommand(qbertGO, Direction::SOUTH_EAST), { Input('S', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Move_NortEast", new MoveCommand(qbertGO, Direction::NORTH_EAST), { Input('D', 0, InputType::BUTTON, InputSource::KEYBOARD) });

	//PrintInfo();
}

void PrintInfo()
{
	std::cout << "\nPress H: to play Hit sound.\n";
	std::cout << "Press M: to play Menu sound.\n";
	std::cout << "Press A: to switch between SDLAudio Service.\n";
	std::cout << "Press S: to switch between logging Audio Service.\n";
}
