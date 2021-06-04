#include "Game.h"

#include <Core.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <ResourceManager.h>

#include "Commands.h"
#include "LevelScene.h"
#include "MainMenu.h"
#include "SdlSoundService.h"
#include "ServiceLocator.h"

using namespace ngenius;

void PrintInfo();
void LoadGame();

int main(int, char* [])
{
	srand(unsigned(time(nullptr)));

	ServiceLocator::RegisterSoundService(new SdlSoundService());
	Core core{ "Q-bert", 960, 720 };
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");	
	LoadGame();
    core.Run();

	ServiceLocator::UnRegister();
    return 0;
}

void LoadGame()
{
	SceneManager::GetInstance().AddScene<LevelScene>("LevelScene1");
	SceneManager::GetInstance().AddScene<MainMenu> ("MainMenu");
	SceneManager::GetInstance().SetCurrentScene("MainMenu");

	//PrintInfo();
}

void PrintInfo()
{
	std::cout << "\nPress H: to play Hit sound.\n";
	std::cout << "Press M: to play Menu sound.\n";
	std::cout << "Press A: to switch between SDLAudio Service.\n";
	std::cout << "Press S: to switch between logging Audio Service.\n";
}
