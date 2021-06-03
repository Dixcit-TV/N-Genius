#pragma once
#include <map>

#include <iostream>
#include "ILoader.h"
#include "Singleton.h"

namespace ngenius
{
	class IResource;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager() override;
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;
		
		void Init(const std::string& data);
		void RegisterLoader(ILoader* resourceLoader);

		template<typename RESOURCE_TYPE, typename... ARG_TYPE>
		std::shared_ptr<RESOURCE_TYPE> LoadResource(const std::string&, ARG_TYPE&&...);
	
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
		std::map<std::string, ILoader*> m_Loaders;

		std::map<std::string, std::shared_ptr<IResource>> m_pResource;
	};

	template<typename RESOURCE_TYPE, typename... ARG_TYPE>
	std::shared_ptr<RESOURCE_TYPE> ResourceManager::LoadResource(const std::string& file, ARG_TYPE&&... args)
	{
		const std::string typeName{ typeid(RESOURCE_TYPE).name() };
		auto loaderIt{ m_Loaders.find(typeName) };

		if (loaderIt != std::end(m_Loaders))
		{
			auto* loader{ static_cast<IBaseLoader<RESOURCE_TYPE, ARG_TYPE...>*>(loaderIt->second) };
			const std::string resourceID{ loader->GenerateResourceID(file, std::forward<ARG_TYPE>(args)...) };

			auto resourceIt{ m_pResource.find(resourceID) };
			if (resourceIt != m_pResource.end())
				return std::static_pointer_cast<RESOURCE_TYPE>(resourceIt->second);
			
			auto newResource{ loader->LoadContent(m_DataPath + file, std::forward<ARG_TYPE>(args)...) };
			m_pResource.emplace(resourceID, newResource);
			return newResource;
		}

		std::cout << "Could not load resource of type " << typeName << std::endl;
		
		return nullptr;
	}
}
