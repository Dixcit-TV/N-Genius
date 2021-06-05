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

enum class CellFace
{
	TOP
	, LEFT
	, RIGHT
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

enum class CharacterState
{
	IDLE, MOVING, ONDISK, DEAD
};

enum class EnemyType
{
	SLICK, SAM, UGG, WRONGWAY, COILY, EGG
};

enum class GameType
{
	SINGLE, COOP, VERSUS
};

enum class GameState
{
	MAIN_MENU, GAME_START, SWITCH_LEVEL, IN_GAME, GAME_OVER
};

