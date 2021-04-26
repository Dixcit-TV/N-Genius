#pragma once
#include <map>
#include <memory>
#include <SDL_image.h>
#include <stdexcept>
#include "Texture2D.h"
#include "Font.h"
#include "Renderer.h"
#include "IResource.h"

namespace Loaders
{
	template<typename RESOURCE_TYPE, typename... ARG_TYPE>
	struct LoaderWrapper
	{
		static std::shared_ptr<RESOURCE_TYPE> Load(std::map<std::string, std::shared_ptr<ngenius::IResource>>&, const std::string& file, const std::string&, ARG_TYPE&&...)
		{
			throw std::logic_error("Function not implemented for resource: " + file);
		}
	};

	template<>
	struct LoaderWrapper<ngenius::Font, int>
	{
		static std::shared_ptr<ngenius::Font> Load(std::map<std::string, std::shared_ptr<ngenius::IResource>>& resourceMap, const std::string& file, const std::string& resourceFolder, int size)
		{
			auto fullName{ file + "_" + std::to_string(size) };
			auto resourceIt{ resourceMap.find(fullName) };
			if (resourceIt != resourceMap.end())
				return std::static_pointer_cast<ngenius::Font>(resourceIt->second);

			auto newResource{ std::make_shared<ngenius::Font>(resourceFolder + file, size) };
			resourceMap.emplace(fullName, newResource);

			return newResource;
		}
	};

	template<>
	struct LoaderWrapper<ngenius::Texture2D>
	{
		static std::shared_ptr<ngenius::Texture2D> Load(std::map<std::string, std::shared_ptr<ngenius::IResource>>& resourceMap, const std::string& file, const std::string& resourceFolder)
		{
			auto resourceIt{ resourceMap.find(file) };
			if (resourceIt != resourceMap.end())
				return std::static_pointer_cast<ngenius::Texture2D>(resourceIt->second);

			const std::string& fullPath{ resourceFolder + file };
			auto texture = IMG_LoadTexture(ngenius::Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
			}
			auto newResource{ std::make_shared<ngenius::Texture2D>(texture) };
			resourceMap.emplace(file, newResource);

			return newResource;
		}
	};
}
