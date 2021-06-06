#pragma once
#include <memory>
#include <string>

#include "Enums.h"
#include "StageData.h"
#include <map>

struct PlayerData
{
	std::string playerName;
	int health;
	int score;
	bool isDead;
};

struct GameData
{
	std::map<std::string, PlayerData> playerData;
	std::shared_ptr<StageData> pStageData;
	int currentLevelIdx;
};

