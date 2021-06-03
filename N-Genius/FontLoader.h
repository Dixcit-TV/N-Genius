#pragma once
#include "Font.h"
#include "ILoader.h"

namespace ngenius
{
	class FontLoader final : public IBaseLoader<Font, int>
	{
	public:
		explicit FontLoader();
		FontLoader(const FontLoader&) = delete;
		FontLoader(FontLoader&&) noexcept = delete;
		FontLoader& operator=(const FontLoader&) = delete;
		FontLoader& operator=(FontLoader&&) noexcept = delete;
		~FontLoader() override = default;

		std::string GenerateResourceID(const std::string& path, int&& fontSize) const override { return path + "_" + std::to_string(fontSize); }
		std::shared_ptr<Font> LoadContent(const std::string& path, int&& fontSize) const override;
	};
}

