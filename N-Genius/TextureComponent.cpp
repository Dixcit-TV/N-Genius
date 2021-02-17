#include "PCH.h"
#include "TextureComponent.h"

#include "GameObject.h"
#include "Renderer.h"

ngenius::TextureComponent::TextureComponent(const std::string& textureName, const TransformComponent& localTransform)
	: IComponent()
	, m_LocalTransform(localTransform)
	, m_pTexture(ResourceManager::GetInstance().LoadTexture(textureName))
{};

void ngenius::TextureComponent::Receive(eComponentMessage message, const std::shared_ptr<GameObject>& pParentGo)
{
	switch (message)
	{
	case eComponentMessage::RENDER:
		Render(*pParentGo->GetTransform());
		break;
	default: break;
	}
}

void ngenius::TextureComponent::Render(const TransformComponent& parentTransform) const
{
	const auto pos = m_LocalTransform.GetPosition();
	const auto parentPos = parentTransform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, parentPos.x + pos.x, parentPos.y + pos.y);
}