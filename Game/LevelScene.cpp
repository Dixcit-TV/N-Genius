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
#include "FactoryMethod.h"

void LevelScene::Initialise()
{
	auto pyramid = std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(200.f, 520.f)), "Pyramid");
	auto pyramidComp = pyramid->AddComponent<Pyramid>(7, 40.f, false, false);
	Add(pyramid);

	auto pfont = ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Font>("Lingua.otf", 20);
	auto scoreTextGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 20.f)), "Score_UI_P1") };
	auto textComp = scoreTextGo->AddComponent<ngenius::TextComponent>("Score: 0", pfont);
	Add(scoreTextGo);

	auto scoreText2Go{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(940.f, 20.f)), "Score_UI_P2") };
	auto textComp2 = scoreText2Go->AddComponent<ngenius::TextComponent>("Score: 0", pfont, SDL_Color{255, 255, 255}, glm::vec2{1.f, 0.f});
	Add(scoreText2Go);

	auto qbert1 = FactoryMethod::CreateQbert(ngenius::Transform(pyramidComp->GetBottomLeftPosition(), glm::vec2(2.f, 2.f))
		, pyramidComp
		, textComp
		, "Player1", PlayerTag::PLAYER1);

	auto qbert2 = FactoryMethod::CreateQbert(ngenius::Transform(pyramidComp->GetBottomRightPosition(), glm::vec2(2.f, 2.f))
		, pyramidComp
		, textComp2
		, "Player2", PlayerTag::PLAYER2);
	
	Add(qbert1);
	Add(qbert2);
}
