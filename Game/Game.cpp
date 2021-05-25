#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <Memory>
#include <Core.h>
#include <SceneManager.h>
#include <Scene.h>

#include <GameObject.h>
#include <InputManager.h>
#include <ResourceManager.h>
#include <TextComponent.h>
#include <TextureComponent.h>

#include "Commands.h"

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

	
	Core core{ "Q-bert", 640, 480 };
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

	auto go = std::make_shared<GameObject>();
	go->AddComponent<TextureComponent>("background.jpg");
	scene.Add(go);

	go = std::make_shared<GameObject>(Transform(216.f, 180.f));
	go->AddComponent<TextureComponent>("logo.png");
	scene.Add(go);

	InputManager::GetInstance().BindInput("Hit_Sound", new PlayHitSoundCommand(nullptr), { Input('H', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Menu_Sound", new PlayMenuSoundCommand(nullptr), { Input('M', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Sdl_Audio_Switch", new SDLAudioCommand(nullptr), { Input('A', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Logging_Audio_Switch", new LoggingAudioCommand(nullptr), { Input('S', 0, InputType::BUTTON, InputSource::KEYBOARD) });

	PrintInfo();
}

void PrintInfo()
{
	std::cout << "\nPress H: to play Hit sound.\n";
	std::cout << "Press M: to play Menu sound.\n";
	std::cout << "Press A: to switch between SDLAudio Service.\n";
	std::cout << "Press S: to switch between logging Audio Service.\n";
}
