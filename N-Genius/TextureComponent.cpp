#include "PCH.h"
#include "TextureComponent.h"

#include "GameObject.h"
#include "Renderer.h"

ngenius::TextureComponent::TextureComponent(const std::string& textureName)
	: IComponent()
	, m_pTexture(ResourceManager::GetInstance().LoadTexture(textureName))
{};

void ngenius::TextureComponent::Render() const
{
	const auto parentPos = m_pGameObject->GetTransform()->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, parentPos.x, parentPos.y);
}