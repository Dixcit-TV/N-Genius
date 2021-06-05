#include "TextureComponent.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Renderer.h"

ngenius::TextureComponent::TextureComponent(const std::string& textureName, const glm::vec2& pivot)
	: IComponent()
	, m_Pivot(pivot)
	, m_pTexture(ResourceManager::GetInstance().LoadResource<Texture2D>(textureName))
{};

void ngenius::TextureComponent::Render() const
{
	const auto& parent = GetTransform();
	Renderer::GetInstance().RenderTexture(m_pTexture->GetSDLTexture(), parent.GetPosition(), m_pTexture->GetTextureSize(), parent.GetScale(), parent.GetRotation(), m_Pivot);
}