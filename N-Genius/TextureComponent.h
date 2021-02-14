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
		explicit TextureComponent(const std::string& textureName, const TransformComponent& localTransform = TransformComponent{});

		void Receive(eComponentMessage message, const std::shared_ptr<GameObject>& pParentGo) override;

		void Render(const TransformComponent& parentTransform) const;

	private:
		TransformComponent m_LocalTransform;
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}

