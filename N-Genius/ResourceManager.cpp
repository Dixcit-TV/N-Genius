#include "PCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

ngenius::ResourceManager::~ResourceManager()
{
	for (auto& loaderPair : m_Loaders)
		delete loaderPair.second;
}

void ngenius::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

void ngenius::ResourceManager::RegisterLoader(ILoader* resourceLoader)
{
	const std::string typeName{ resourceLoader->GetType().name() };

	if (!m_Loaders.try_emplace(typeName, resourceLoader).second)
	{
		std::cout << "could not add loader for type " << typeName << ", it might already have been registered.";
	}
}