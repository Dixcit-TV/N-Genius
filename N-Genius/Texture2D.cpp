#include "PCH.h"
#include "Texture2D.h"
#include <SDL.h>

ngenius::Texture2D::Texture2D(SDL_Texture* texture)
	: m_TextureSize()
	, m_Texture(texture)
{
	SDL_QueryTexture(m_Texture, NULL, NULL, &m_TextureSize.x, &m_TextureSize.y);
}

ngenius::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* ngenius::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

const glm::ivec2& ngenius::Texture2D::GetTextureSize() const
{
	return m_TextureSize;
}