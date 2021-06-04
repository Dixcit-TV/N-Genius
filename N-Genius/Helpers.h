#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>
#pragma warning(pop)

namespace Helpers
{
	bool AabbOverlapCheck(const glm::vec2& centerPosition1, float width1, float height1,
		const glm::vec2& centerPosition2, float width2, float height2);
}

