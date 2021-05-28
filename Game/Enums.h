#pragma once
enum class CellState
{
	INITIAL
	, INTERMEDIATE
	, FINAL
};

enum class Direction
{
	NORTH_EAST
	, NORTH_WEST
	, SOUTH_EAST
	, SOUTH_WEST
};

enum class QBertState
{
	IDLE, MOVING, ONDISK, DEAD
};

