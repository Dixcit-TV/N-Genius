#include "MainMenu.h"

#include "FactoryMethod.h"
#include "InputManager.h"

void MainMenu::Initialise()
{
	ngenius::InputManager::GetInstance().AddSDLButtonListener(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT);

	auto singleButton{ FactoryMethod::CreateButton("Start Single", glm::vec2{ 100.f, 200.f}) };
	auto coopButton{ FactoryMethod::CreateButton("Start Co-op", glm::vec2{ 100.f, 400.f}) };
	auto versusButton{ FactoryMethod::CreateButton("Start Versus", glm::vec2{ 100.f, 600.f}) };

	Add(singleButton);
	Add(coopButton);
	Add(versusButton);
}
