#include <SDL_ttf.h>
#include "Font.h"

#include <stdexcept>

ngenius::Font::Font(const std::string& fullPath, unsigned int size)
	: m_Font(TTF_OpenFont(fullPath.c_str(), size))
	, m_Size(size)
{
	if (m_Font == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

ngenius::Font::~Font()
{
	TTF_CloseFont(m_Font);
}

TTF_Font* ngenius::Font::GetFont() const
{
	return m_Font;
}