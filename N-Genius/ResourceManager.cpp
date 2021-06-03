#include "PCH.h"
#include "ResourceManager.h"

ngenius::ResourceManager::~ResourceManager()
{
	for (auto& loaderPair : m_Loaders)
		delete loaderPair.second;
}

void ngenius::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;
}

void ngenius::ResourceManager::RegisterLoader(ILoader* resourceLoader)
{
	const std::string typeName{ resourceLoader->GetType().name() };

	if (!m_Loaders.try_emplace(typeName, resourceLoader).second)
	{
		std::cout << "could not add loader for type " << typeName << ", it might already have been registered.";
	}
}