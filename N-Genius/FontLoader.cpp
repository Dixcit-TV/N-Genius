#include "FontLoader.h"

std::shared_ptr<ngenius::Font> ngenius::FontLoader<int>::LoadContent(const std::string& path, int&& fontSize) const
{
	return std::make_shared<Font>(path, fontSize);
}