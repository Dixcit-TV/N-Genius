#include "Texture2DLoader.h"
#include <SDL_image.h>
#include <stdexcept>

#include "Renderer.h"

std::shared_ptr<ngenius::Texture2D> ngenius::Texture2DLoader::LoadContent(const std::string& path) const
{
	const std::string& fullPath{ path };
	SDL_Texture* texture = IMG_LoadTexture(ngenius::Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	auto newResource{ std::make_shared<ngenius::Texture2D>(texture) };

	return newResource;
}
