#include "Helpers.h"

bool Helpers::AabbOverlapCheck(const glm::vec2& centerPosition1, float width1, float height1,
	const glm::vec2& centerPosition2, float width2, float height2)
{
	const float halfWidth1{ width1 * 0.5f };
	const float halfWidth2{ width2 * 0.5f };
	const float halfHeight1{ height1 * 0.5f };
	const float halfHeight2{ height2 * 0.5f };
	
	if (centerPosition1.x - halfWidth1 > centerPosition2.x + halfWidth2
		|| centerPosition2.x - halfWidth2 > centerPosition1.x + halfWidth1)
		return false;
	
	if (centerPosition1.y - halfHeight1 > centerPosition2.y + halfHeight2
		|| centerPosition2.y - halfHeight2 > centerPosition1.y + halfHeight1)
		return false;

	return true;
}