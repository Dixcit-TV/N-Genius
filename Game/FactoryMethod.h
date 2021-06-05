#pragma once
#include <memory>
#include <string>

#include "Enums.h"
#include "RigidBody.h"

class EnemySpawner;
class Pyramid;

namespace ngenius {
	class TextComponent;
	class Transform;
	class GameObject;
}

namespace FactoryMethod
{
#pragma region GameObjects
	std::shared_ptr<ngenius::GameObject> CreatePyramid();
	
	std::shared_ptr<ngenius::GameObject> CreateQbert(const ngenius::Transform& transform, std::shared_ptr<Pyramid> pyramidComp, std::shared_ptr<ngenius::TextComponent> scoreTextComp, PlayerTag tag);
	std::shared_ptr<ngenius::GameObject> CreateSlickSam(const ngenius::Transform& transform, std::shared_ptr<Pyramid> pyramidComp, std::shared_ptr<EnemySpawner> spawner, EnemyType type);
	std::shared_ptr<ngenius::GameObject> CreateEgg(const ngenius::Transform& transform, std::shared_ptr<EnemySpawner> spawner, EnemyType type = EnemyType::EGG);
	std::shared_ptr<ngenius::GameObject> CreateCoily(const ngenius::Transform& transform, std::shared_ptr<EnemySpawner> spawner, EnemyType type = EnemyType::COILY);
	std::shared_ptr<ngenius::GameObject> CreateUggWrongWay(const ngenius::Transform& transform, std::shared_ptr<EnemySpawner> spawner, EnemyType type);
	std::shared_ptr<ngenius::GameObject> CreateButton(const std::string& text, const glm::vec2& position, const std::string& name = "Button", int textSize = 28);
#pragma endregion

#pragma region CollisionResponses
	void OverlapResponsePlayerEnemy(std::shared_ptr<ngenius::RigidBody> thisBody, std::shared_ptr<ngenius::RigidBody> overlappingBody);
	void OverlapResponsePlayerSlickSam(std::shared_ptr<ngenius::RigidBody> thisBody, std::shared_ptr<ngenius::RigidBody> overlappingBody);
#pragma endregion

#pragma region Scenes
	void InitMainMenu(std::shared_ptr<ngenius::Scene> pScene);
	void InitSinglePlayerScene(std::shared_ptr<ngenius::Scene> pScene);
	void InitCoopScene(std::shared_ptr<ngenius::Scene> pScene);
	void InitVersusScene(std::shared_ptr<ngenius::Scene> pScene);
	void InitGameOverScene(std::shared_ptr<ngenius::Scene> pScene);
#pragma endregion
}

