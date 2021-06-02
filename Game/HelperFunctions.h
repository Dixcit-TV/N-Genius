#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>

#include "Enums.h"
#pragma warning(pop)

namespace Helpers
{
	glm::vec2 EnumToDirection(Direction direction, CellFace face = CellFace::TOP);

	int RandValue(int min = 0, int max = 1);
	float RandValue(float min = 0.f, float max = 1.f);
}
