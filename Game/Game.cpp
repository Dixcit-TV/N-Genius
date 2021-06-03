#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <Core.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <ResourceManager.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "Commands.h"
#include "LevelScene.h"
#include "ServiceLocator.h"

using namespace ngenius;

void PrintInfo();
void LoadGame();

int main(int, char* [])
{
	srand(unsigned(time(nullptr)));
	
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
	SceneManager::GetInstance().AddScene<LevelScene>("LevelScene1");
	SceneManager::GetInstance().SetCurrentScene("LevelScene1");

	//PrintInfo();
}

void PrintInfo()
{
	std::cout << "\nPress H: to play Hit sound.\n";
	std::cout << "Press M: to play Menu sound.\n";
	std::cout << "Press A: to switch between SDLAudio Service.\n";
	std::cout << "Press S: to switch between logging Audio Service.\n";
}
