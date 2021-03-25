#include "PCH.h"
#include "Core.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "TextComponent.h"
#include "TimeSettings.h"
#include "TextureComponent.h"
#include "PhysicsComponent.h"
#include "ScoreComponent.h"

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
	SceneManager::GetInstance().SetCurrentScene(0);

	auto go = std::make_shared<GameObject>();
	go->AddComponent<TextureComponent>("background.jpg");
	scene.Add(go);

	go = std::make_shared<GameObject>(Transform(216.f, 180.f));
	go->AddComponent<TextureComponent>("logo.png");
	scene.Add(go);
	
	auto pfont = ResourceManager::GetInstance().LoadResource<Font>("Lingua.otf", 20);
	auto pGoFps = std::make_shared<GameObject>(Transform(5.f, 5.f));
	pGoFps->AddComponent<TextComponent>("", pfont, SDL_Color{ 196, 174, 29 });
	scene.Add(pGoFps);

	//QBERT1 
	auto QbertGo = std::make_shared<GameObject>();
	auto healthComponent{ QbertGo->AddComponent<HealthComponent>(5) };
	auto physicsComponent{ QbertGo->AddComponent<PhysicsComponent>() };
	auto scoreComponent{ QbertGo->AddComponent<ScoreComponent>() };
	physicsComponent->AddCollisionListener(healthComponent);
	physicsComponent->AddCollisionListener(scoreComponent);

	//QBERT1 UI
	pfont = ResourceManager::GetInstance().LoadResource<Font>("Lingua.otf", 20);
	auto pGoHealth1 = std::make_shared<GameObject>(Transform(5.f, 450.f));
	pGoHealth1->AddComponent<TextComponent>("", pfont, SDL_Color{ 196, 174, 29 });
	scene.Add(pGoHealth1);

	auto pGoScore1 = std::make_shared<GameObject>(Transform(5.f, 410.f));
	pGoScore1->AddComponent<TextComponent>("", pfont, SDL_Color{ 196, 174, 29 });
	scene.Add(pGoScore1);

	//QBERT2
	auto Qbert2Go = std::make_shared<GameObject>();
	healthComponent = Qbert2Go->AddComponent<HealthComponent>(5);
	physicsComponent = Qbert2Go->AddComponent<PhysicsComponent>();
	scoreComponent = Qbert2Go->AddComponent<ScoreComponent>();
	physicsComponent->AddCollisionListener(healthComponent);
	physicsComponent->AddCollisionListener(scoreComponent);

	//QBERT2 UI
	auto pGoHealth2 = std::make_shared<GameObject>(Transform(500.f, 450.f));
	pGoHealth2->AddComponent<TextComponent>("", pfont, SDL_Color{ 196, 174, 29 });
	scene.Add(pGoHealth2);

	auto pGoScore2 = std::make_shared<GameObject>(Transform(500.f, 410.f));
	pGoScore2->AddComponent<TextComponent>("", pfont, SDL_Color{ 196, 174, 29 });
	scene.Add(pGoScore2);

	InputManager::GetInstance().BindInput("Player_Kill", new KillPlayerCommand(QbertGo), { Input('K', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Player_Move", new JumpCommand(QbertGo), { Input('W', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Player_Attack", new AttackCommand(QbertGo), { Input(VK_SPACE, 0, InputType::BUTTON, InputSource::KEYBOARD) });
	InputManager::GetInstance().BindInput("Player_Catch", new CatchCommand(QbertGo), { Input('R', 0, InputType::BUTTON, InputSource::KEYBOARD) });
	const int newGamePadId{ InputManager::GetInstance().RegisterGamepad() };
	InputManager::GetInstance().BindInput("Player2_Kill", new KillPlayerCommand(Qbert2Go), { Input(XINPUT_GAMEPAD_Y, 0, InputType::BUTTON, InputSource::GAMEPAD, newGamePadId) });
	InputManager::GetInstance().BindInput("Player2_Move", new JumpCommand(Qbert2Go), { Input(XINPUT_GAMEPAD_DPAD_UP, 0, InputType::BUTTON, InputSource::GAMEPAD, newGamePadId) });
	InputManager::GetInstance().BindInput("Player2_Attack", new AttackCommand(Qbert2Go), { Input(XINPUT_GAMEPAD_A, 0, InputType::BUTTON, InputSource::GAMEPAD, newGamePadId) });
	InputManager::GetInstance().BindInput("Player2_Catch", new CatchCommand(Qbert2Go), { Input(XINPUT_GAMEPAD_B, 0, InputType::BUTTON, InputSource::GAMEPAD, newGamePadId) });

	PrintCommands();
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
			sceneManager.Update();
			renderer.Render();

			timeSettings.TrySleep();
		}
	}

	Cleanup();
}

void ngenius::Core::PrintCommands() const
{
	std::cout << "----------- HOW TO PLAY ----------\n";
	std::cout << "Player1:\n";
	std::cout << "	- Kill player: K\n";
	std::cout << "	- Change Color Points: W\n";
	std::cout << "	- Kill Coily: SPACE\n";
	std::cout << "	- Catch: R\n";
	std::cout << "\n";
	std::cout << "Player2:\n";
	std::cout << "	- Kill player: GAMEPAD FACE UP\n";
	std::cout << "	- Change Color Points: GAMEPAD DPAD UP\n";
	std::cout << "	- Kill Coily: GAMEPAD FACE DOWN\n";
	std::cout << "	- Catch: GAMEPAD FACE RIGHT\n";
}
