#include "SpriteSheet.h"

ngenius::SpriteSheet::SpriteSheet(SDL_Texture* texture, uint32_t frameCount, uint32_t columnCount)
	: Texture2D(texture)
	, m_FrameSize(m_TextureSize.x / columnCount, m_TextureSize.y * (columnCount / frameCount))
	, m_ColumnCount(columnCount)
	, m_FrameCount(frameCount)
{}