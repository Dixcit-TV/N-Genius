#include "HelperFunctions.h"

glm::vec2 Helpers::EnumToDirection(Direction direction)
{
	switch (direction)
	{
	case Direction::NORTH_EAST:
		return { 0, 1 };
	case Direction::NORTH_WEST:
		return { -1, 1 };
	case Direction::SOUTH_EAST:
		return { 1, -1 };
	case Direction::SOUTH_WEST:
		return { 0, -1 };
	}

	return {};
}