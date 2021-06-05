#pragma once
#include <ILoader.h>
#include "StageData.h"

class StageLoader : public ngenius::IBaseLoader<StageData>
{
public:
	explicit StageLoader() = default;
	StageLoader(const StageLoader&) = delete;
	StageLoader(StageLoader&&) noexcept = delete;
	StageLoader& operator=(const StageLoader&) = delete;
	StageLoader& operator=(StageLoader&&) noexcept = delete;
	~StageLoader() override = default;

	std::shared_ptr<StageData> LoadContent(const std::string & path) const override;
};

