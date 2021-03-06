#pragma once
#include "Singleton.h"

namespace ngenius
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager() override = default;
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;
		
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};
}
