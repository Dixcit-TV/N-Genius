#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>

#include "Enums.h"
#pragma warning(pop)

namespace Helpers
{
	glm::vec2 EnumToDirection(Direction direction);
}
