#include "LevelScene.h"
#include <GameObject.h>
#include <ResourceManager.h>
#include <Font.h>

#include "EnemySpawner.h"
#include "Pyramid.h"
#include "TextComponent.h"
#include "FactoryMethod.h"
#include "LifeComponent.h"
#include "LifeDisplay.h"

void LevelScene::Initialise()
{
	auto pyramid = std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(200.f, 600.f)), "Pyramid");
	auto pyramidComp = pyramid->AddComponent<Pyramid>(7, 40.f, false, false);
	pyramid->AddComponent<EnemySpawner>();
	Add(pyramid);

	auto pfont = ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Font>("Lingua.otf", 20);
	auto scoreTextGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 20.f)), "Score_UI_P1") };
	auto textComp = scoreTextGo->AddComponent<ngenius::TextComponent>("Score: 0", pfont);
	Add(scoreTextGo);

	auto scoreText2Go{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(940.f, 20.f)), "Score_UI_P2") };
	auto textComp2 = scoreText2Go->AddComponent<ngenius::TextComponent>("Score: 0", pfont, SDL_Color{255, 255, 255}, glm::vec2{1.f, 0.f});
	Add(scoreText2Go);

	auto qbert1 = FactoryMethod::CreateQbert(ngenius::Transform(pyramidComp->GetBottomLeftPosition())
		, pyramidComp
		, textComp
		, "Player1", PlayerTag::PLAYER1);

	auto qbert2 = FactoryMethod::CreateQbert(ngenius::Transform(pyramidComp->GetBottomRightPosition())
		, pyramidComp
		, textComp2
		, "Player2", PlayerTag::PLAYER2);
	
	Add(qbert1);
	Add(qbert2);

	auto LifeDisplaytGo{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(20.f, 60.f)), "Life_UI_P1") };
	auto lifeDisplayComp = LifeDisplaytGo->AddComponent<LifeDisplay>(3);
	qbert1->GetComponent<LifeComponent>()->RegisterUpdateEvent("UpdateDisplayEvent", Make_Delegate(std::weak_ptr(lifeDisplayComp), &LifeDisplay::UpdateLifeDrawCount));
	Add(LifeDisplaytGo);

	auto LifeDisplayt2Go{ std::make_shared<ngenius::GameObject>(ngenius::Transform(glm::vec2(890.f, 60.f)), "Life_UI_P2") };
	auto lifeDisplay2Comp = LifeDisplayt2Go->AddComponent<LifeDisplay>(3);
	qbert2->GetComponent<LifeComponent>()->RegisterUpdateEvent("UpdateDisplayEvent", Make_Delegate(std::weak_ptr(lifeDisplay2Comp), &LifeDisplay::UpdateLifeDrawCount));
	Add(LifeDisplayt2Go);
}
