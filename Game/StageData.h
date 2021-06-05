#pragma once
#include "IResource.h"
#include <string>
#include <vector>

struct LevelData
{
	static const int COLORSCHEMECOUNT{ 9 };
	std::string name{};
	int rowCount{ 0 };
	int ColorSchemeIndex{ 0 };
	bool hasIntermediateColor{ false };
	bool colorRverting{ false };
};

struct StageData : ngenius::IResource
{
	std::vector<LevelData> levels{};
};

