#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "IComponent.h"

ngenius::Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_Objects()
	, m_Initalized(false)
{}

void ngenius::Scene::Add(const std::shared_ptr<GameObject>& object, bool force)
{
	assert(object->m_pParentScene.expired() || object->m_pParentScene.lock() == nullptr);

	if (m_Initalized && !force)
		m_SceneQueue.push_back([object, this]() { Add(object, true); });
	else
	{
		m_Objects.push_back(object);
		object->m_pParentScene = weak_from_this();
	}
}

void ngenius::Scene::Remove(const std::shared_ptr<GameObject>& object, bool force)
{
	if (!(object->m_pParentScene.lock() == shared_from_this()))
	{
		std::cout << "GameObject " << object->m_Name << " is not attached to scene " << m_Name << " and cannot be removed." << std::endl;
	}
	
	if (m_Initalized && !force)
		m_SceneQueue.push_back([object, this]() { Remove(object, true); });
	else
	{
		auto it{ std::find(std::begin(m_Objects), std::end(m_Objects), object) };
		object->m_pParentScene.reset();
		m_Objects.erase(it);
	}
}

void ngenius::Scene::Update()
{
	for (auto event : m_SceneQueue)
		event();
	
	m_SceneQueue.clear();
	
	for(auto object : m_Objects)
	{
		const GameObject::Components& components{ object->GetAllComponents() };
		for (auto comp : components)
		{
			comp->Update();
		}
	}
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

