#include "Texture2DLoader.h"
#include <SDL_image.h>
#include <stdexcept>

#include "Renderer.h"

ngenius::Texture2DLoader::Texture2DLoader()
	: IBaseLoader<Texture2D>()
{
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}
}

std::shared_ptr<ngenius::Texture2D> ngenius::Texture2DLoader::LoadContent(const std::string& path) const
{
	const std::string& fullPath{ path };
	SDL_Texture* texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	auto newResource{ std::make_shared<Texture2D>(texture) };

	return newResource;
}
