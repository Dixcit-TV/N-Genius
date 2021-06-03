#pragma once
#include "ILoader.h"
#include "Texture2D.h"

namespace ngenius
{
	class Texture2DLoader final : public IBaseLoader<Texture2D>
	{
	public:
		explicit Texture2DLoader();
		Texture2DLoader(const Texture2DLoader&) = delete;
		Texture2DLoader(Texture2DLoader&&) noexcept = delete;
		Texture2DLoader& operator=(const Texture2DLoader&) = delete;
		Texture2DLoader& operator=(Texture2DLoader&&) noexcept = delete;
		~Texture2DLoader() override = default;

		std::shared_ptr<Texture2D> LoadContent(const std::string& path) const override;
	};
}

