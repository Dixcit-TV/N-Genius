#pragma once
#include "Font.h"
#include "ILoader.h"

namespace ngenius
{
	template<typename PARAM_TYPE = int>
	class FontLoader final : public IBaseLoader<Font, int>
	{
	public:
		explicit FontLoader() = default;
		FontLoader(const FontLoader&) = delete;
		FontLoader(FontLoader&&) noexcept = delete;
		FontLoader& operator=(const FontLoader&) = delete;
		FontLoader& operator=(FontLoader&&) noexcept = delete;
		~FontLoader() override = default;

		std::string GenerateResourceID(const std::string& path, PARAM_TYPE&& fontSize) const override { return path + "_" + std::to_string(fontSize); }
		std::shared_ptr<Font> LoadContent(const std::string& path, PARAM_TYPE&& fontSize) const override;
	};
}

