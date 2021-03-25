#pragma once
#include <map>
#include "Singleton.h"
#include "ResourceLoaders.h"

namespace ngenius
{
	class IResource;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager() override = default;
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;
		
		void Init(const std::string& data);

		template<typename RESOURCE_TYPE, typename... ARG_TYPE>
		std::shared_ptr<RESOURCE_TYPE> LoadResource(const std::string&, ARG_TYPE&&...);
	
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;

		std::map<std::string, std::shared_ptr<IResource>> m_pResource;
	};

	template<typename RESOURCE_TYPE, typename... ARG_TYPE>
	std::shared_ptr<RESOURCE_TYPE> ResourceManager::LoadResource(const std::string& file, ARG_TYPE&&... args)
	{
		return Loaders::LoaderWrapper<RESOURCE_TYPE, ARG_TYPE...>::Load(m_pResource, file, m_DataPath, std::forward<ARG_TYPE>(args)...);
	}
}
