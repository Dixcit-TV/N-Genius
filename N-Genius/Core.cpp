#include "PCH.h"
#include "Core.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "TextComponent.h"
#include "TimeSettings.h"
#include "TextureComponent.h"

using namespace std;
using namespace std::chrono;

void ngenius::Core::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 - N-Genius",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void ngenius::Core::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	go->AddComponent<TextureComponent>("background.jpg");
	go->AddComponent<TransformComponent>(0.f, 0.f);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->AddComponent<TextureComponent>("logo.png");
	go->AddComponent<TransformComponent>(216.f, 180.f);
	scene.Add(go);

	auto pfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pGoFps = std::make_shared<GameObject>();
	pGoFps->AddComponent<TextComponent>("", []() { return std::to_string(TimeSettings::GetInstance().GetFPS()) + " FPS"; }, pfont, TransformComponent(20.f, 20.f));
	pGoFps->AddComponent<TransformComponent>(0.f, 0.f);
	scene.Add(pGoFps);
}

void ngenius::Core::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void ngenius::Core::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();
	
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& timeSettings = TimeSettings::GetInstance();

		bool doContinue = true;
		while (doContinue)
		{
			timeSettings.Update();
			
			doContinue = input.ProcessInput();

			while(timeSettings.CatchUp())
			{
				sceneManager.Update();
			}
			
			renderer.Render();
		}
	}

	Cleanup();
}
