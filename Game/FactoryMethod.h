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
}

