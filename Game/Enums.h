#pragma once
enum class PlayerTag
{
	PLAYER1
	, PLAYER2
};

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

enum class ScoreEventType
{
	COLOR_CHANGE
	, COILY_DEFEAT
	, REMAINING_DISK
	, SLICK_SAM_CAUGHT
};

enum class QBertState
{
	IDLE, MOVING, ONDISK, DEAD
};

