#pragma once
enum class eAnchorMode
{
	TOP_LEFT, TOP_CENTER, TOP_RIGHT, MIDDLE_LEFT, MIDDLE_CENTER, MIDDLE_RIGHT, BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT
};

enum class eComponentMessage
{
	UPDATE, RENDER
};

enum class Event
{
	PLAYER_HIT, BLOCK_HIT, COILY_HIT, REMAINING_DISKS, SLICK_SAM_CAUGHT
};