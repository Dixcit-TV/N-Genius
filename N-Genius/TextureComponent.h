#pragma once
#include "IComponent.h"
#include "ResourceManager.h"

namespace ngenius
{
	class Texture2D;
	class TextureComponent final : public IComponent
	{
	public:
		explicit TextureComponent(const std::string& textureName, const glm::vec2& pivot = { 0.f, 0.f });
		~TextureComponent() override = default;
		TextureComponent(const TextureComponent& rhs) = delete;
		TextureComponent(TextureComponent&& rhs) noexcept = delete;
		TextureComponent& operator=(const TextureComponent& rhs) = delete;
		TextureComponent& operator=(TextureComponent&& rhs) noexcept = delete;

		void Render() const override;

	private:
		glm::vec2 m_Pivot;
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}

