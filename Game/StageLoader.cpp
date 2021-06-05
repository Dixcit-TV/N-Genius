#include "StageLoader.h"
#include <fstream>
#include <json.hpp>

#include "HelperFunctions.h"

std::shared_ptr<StageData> StageLoader::LoadContent(const std::string& path) const
{
	std::ifstream ifs{path, std::ios::in };

	if (!ifs.is_open())
		return nullptr;

	std::string jsonStr((std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>());
	
	auto jsonObject = nlohmann::json::parse(jsonStr);
	auto pStageData{std::make_shared<StageData>()};
	pStageData->levels.reserve(jsonObject.size());
	for (auto& level : jsonObject)
	{
		int colorIndex{ level["ColorScheme"] };
		if (colorIndex == -1)
			colorIndex = Helpers::RandValue(0, LevelData::COLORSCHEMECOUNT - 1);
		
		pStageData->levels.push_back(LevelData{ level["Name"], level["RowCount"], colorIndex, level["IntermediateColor"], level["RevertColor"] });
	}

	return pStageData;
}
