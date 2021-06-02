#include "LifeDisplay.h"

#include "ResourceManager.h"

LifeDisplay::LifeDisplay(int initialLifeCount)
	: IComponent()
	, m_pLifeTexture(ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Texture2D>("Sprites/Character.png"))
	, m_LifeCount(initialLifeCount)
{}

void LifeDisplay::UpdateLifeDrawCount(int, int newLifeCount)
{
	m_LifeCount = newLifeCount;
}

void LifeDisplay::Render() const
{
	const glm::vec2& position{ GetTransform().GetPosition() };
	const glm::vec2& texSize{ m_pLifeTexture->GetTextureSize() };
	const int offset{ 20 };
	for (int i{}; i < m_LifeCount; ++i)
	{
		const int col{ i % 2 };
		const int row{ i / 2 };

		const glm::vec2 drawPos{ position.x + (texSize.x + offset) * col, position.y + (texSize.y + offset) * row };

		ngenius::Renderer::GetInstance().RenderTexture(m_pLifeTexture->GetSDLTexture(), drawPos, texSize);
	}
}
