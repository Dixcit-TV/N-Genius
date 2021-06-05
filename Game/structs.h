#pragma once
#include <memory>
#include <string>

#include "Enums.h"
#include "StageData.h"

struct PlayerData
{
	std::string playerName;
	int health;
	int score;
};

struct GameData
{
	PlayerData player1;
	PlayerData player2;
	std::shared_ptr<StageData> pStageData;
	int currentLevelIdx;
};

