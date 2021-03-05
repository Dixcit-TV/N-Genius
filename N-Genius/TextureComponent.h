#pragma once
#include "IComponent.h"
#include "ResourceManager.h"
#include "TransformComponent.h"

namespace ngenius
{
	class Texture2D;
	class TextureComponent final : public IComponent
	{
	public:
		explicit TextureComponent() = default;
		explicit TextureComponent(const std::string& textureName);
		~TextureComponent() override = default;
		TextureComponent(const TextureComponent& rhs) = delete;
		TextureComponent(TextureComponent&& rhs) noexcept = delete;
		TextureComponent& operator=(const TextureComponent& rhs) = delete;
		TextureComponent& operator=(TextureComponent&& rhs) noexcept = delete;

		void Render() const override;

	private:
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}

