#include "Core.h"
#include "SceneManager.h"
#include "Renderer.h"
#include <SDL.h>

#include "FontLoader.h"
#include "GameModeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Texture2DLoader.h"
#include "TimeSettings.h"

using namespace std;
using namespace std::chrono;

void ngenius::Core::Initialize(const std::string& windowName, int width, int height)
{
	if (m_IsInit)
		return;
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		windowName.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);
	
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
	ResourceManager::GetInstance().RegisterLoader(new Texture2DLoader());
	ResourceManager::GetInstance().RegisterLoader(new FontLoader());

	m_IsInit = true;
}

void ngenius::Core::Cleanup()
{
	GameModeManager::Destroy();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void ngenius::Core::Run()
{
	if (!m_IsInit)
	{
		std::cerr << "Core::Run > Core not Initialized";
	}
	
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
			GameModeManager::Update();
			
			sceneManager.Update();
			renderer.Render();

			timeSettings.TrySleep();
		}
	}

	Cleanup();
}
