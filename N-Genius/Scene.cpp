#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"

ngenius::Scene::Scene(const std::string& name) : m_Name(name) {}

void ngenius::Scene::Add(const std::shared_ptr<GameObject>& object)
{
	assert(object->m_pParentScene == nullptr);
	
	m_Objects.push_back(object);
	object->m_pParentScene = shared_from_this();
}

void ngenius::Scene::Update()
{
	for(auto object : m_Objects)
	{
		const GameObject::Components& components{ object->GetAllComponents() };
		for (auto comp : components)
		{
			comp->Update();
		}
	}

	m_Objects.erase(std::remove_if(std::begin(m_Objects), std::end(m_Objects), [](std::shared_ptr<GameObject> pGO)
	{
		return pGO->IsMarkedForDeletion();
	}), std::end(m_Objects));
}

void ngenius::Scene::Render() const
{
	for (auto& object : m_Objects)
	{
		const GameObject::Components& components{ object->GetAllComponents() };
		for (const auto& comp : components)
		{
			comp->Render();
		}
	}
}

std::shared_ptr<ngenius::GameObject> ngenius::Scene::GetGameObjectWithName(const std::string& name) const
{
	const auto it{ std::find_if(std::begin(m_Objects), std::end(m_Objects), [&name](std::shared_ptr<GameObject> pGo) { return pGo->m_Name == name; }) };

	return it == std::end(m_Objects) ? nullptr : *it;
}

std::vector<std::shared_ptr<ngenius::GameObject>> ngenius::Scene::GetAllGameObjectsWithName(const std::string& name) const
{
	std::vector<std::shared_ptr<GameObject>> result;
	const auto it{ std::copy_if(std::begin(m_Objects), std::end(m_Objects), std::back_inserter(result)
		, [&name](std::shared_ptr<GameObject> pGo) { return pGo->m_Name == name; }) };

	return result;
}

