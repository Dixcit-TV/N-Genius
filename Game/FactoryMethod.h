#pragma once
#include <memory>
#include <string>
#include "Enums.h"

class Pyramid;

namespace ngenius {
	class TextComponent;
	class Transform;
	class GameObject;
}

namespace FactoryMethod
{
	std::shared_ptr<ngenius::GameObject> CreateQbert(const ngenius::Transform& transform, std::shared_ptr<Pyramid> pyramidComp, std::shared_ptr<ngenius::TextComponent> scoreTextComp, const std::string& name, PlayerTag tag);
	std::shared_ptr<ngenius::GameObject> CreateSlickSam(const ngenius::Transform& transform, std::shared_ptr<Pyramid> pyramidComp, EnemyType type);
	std::shared_ptr<ngenius::GameObject> CreateEgg(const ngenius::Transform& transform, EnemyType type = EnemyType::EGG);
	std::shared_ptr<ngenius::GameObject> CreateCoily(const ngenius::Transform& transform, EnemyType type = EnemyType::COILY);
	std::shared_ptr<ngenius::GameObject> CreateUggWrongWay(const ngenius::Transform& transform, EnemyType type);
}

