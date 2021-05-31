#include "HelperFunctions.h"

glm::vec2 Helpers::EnumToDirection(Direction direction, CellFace face)
{
	glm::vec2 dir{};
	switch (direction)
	{
	case Direction::NORTH_EAST:
		dir = { 0, 1 };
		break;
	case Direction::NORTH_WEST:
		dir = { -1, 1 };
		break;
	case Direction::SOUTH_EAST:
		dir = { 1, -1 };
		break;
	case Direction::SOUTH_WEST:
		dir = { 0, -1 };
		break;
	}
	
	switch(face)
	{
	case CellFace::LEFT:
		{
			const float cAlpha{ cosf(glm::radians(90.f)) };
			const float sAlpha{ sinf(glm::radians(90.f)) };
			dir = glm::mat2x2(cAlpha, sAlpha, -sAlpha, cAlpha) * dir;
		}
		break;
	case CellFace::RIGHT:
		{
			switch (direction)
			{
			case Direction::NORTH_EAST:
				dir = { 1, -1 };
				break;
			case Direction::NORTH_WEST:
				dir = { 1, 0 };
				break;
			case Direction::SOUTH_EAST:
				dir = { -1, 0 };
				break;
			case Direction::SOUTH_WEST:
				dir = { -1, 1 };
				break;
			}
		}
		break;
	default: break;
	}

	return dir;
}