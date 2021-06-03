#include "FontLoader.h"
#include <SDL_ttf.h>
#include <stdexcept>

ngenius::FontLoader::FontLoader()
	: IBaseLoader<Font, int>()
{
	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<ngenius::Font> ngenius::FontLoader::LoadContent(const std::string& path, int&& fontSize) const
{
	return std::make_shared<Font>(path, fontSize);
}