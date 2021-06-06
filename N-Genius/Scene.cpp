#include "Scene.h"

#include <iostream>
#include "GameObject.h"
#include "IComponent.h"
#include "RigidBody.h"

ngenius::Scene::Scene(const std::string& name)
	: m_PhysxHandler()
	, m_Name(name)
	, m_Objects()
	, m_Initalized(false)
{}

void ngenius::Scene::Clear()
{
	m_PhysxHandler.Clear();
	
	while (!m_SceneQueue.empty())
		m_SceneQueue.pop();
	
	m_Objects.clear();
}

void ngenius::Scene::Add(const std::shared_ptr<GameObject>& object, bool force)
{
	assert(object->m_pParentScene.expired() || object->m_pParentScene.lock() == nullptr);

	if (m_Initalized && !force)
		m_SceneQueue.push([object, this]() { Add(object, true); });
	else
	{
		m_Objects.push_back(object);
		object->m_pParentScene = weak_from_this();
		m_PhysxHandler.RegisterRigidBody(object);
	}
}

void ngenius::Scene::Remove(const std::shared_ptr<GameObject>& object, bool force)
{
	if (!(object->m_pParentScene.lock() == shared_from_this()))
	{
		std::cout << "GameObject " << object->m_Name << " is not attached to scene " << m_Name << " and cannot be removed." << std::endl;
	}
	
	if (m_Initalized && !force)
		m_SceneQueue.push([object, this]() { Remove(object, true); });
	else
	{
		auto it{ std::find(std::begin(m_Objects), std::end(m_Objects), object) };
		m_PhysxHandler.UnregisterRigidBody(object);
		object->m_pParentScene.reset();
		
		m_Objects.erase(it);
	}
}

void ngenius::Scene::Update()
{
	while (!m_SceneQueue.empty())
	{
		m_SceneQueue.front()();
		m_SceneQueue.pop();
	}
	
	for(auto object : m_Objects)
	{
		if (!object->IsEnabled())
			continue;
			
		const GameObject::Components& components{ object->GetAllComponents() };
		for (auto comp : components)
		{
			comp->Update();
		}
	}

	m_PhysxHandler.Update();
}

void ngenius::Scene::Render() const
{
	for (auto& object : m_Objects)
	{
		if (!object->IsEnabled())
			continue;
		
		const GameObject::Components& components{ object->GetAllComponents() };
		for (const auto& comp : components)
		{
			comp->Render();
		}
	}

	m_PhysxHandler.Render();
}

std::shared_ptr<ngenius::GameObject> ngenius::Scene::GetGameObjectWithName(const std::string& name) const
{
	const auto it{ std::find_if(std::begin(m_Objects), std::end(m_Objects), [&name](std::shared_ptr<GameObject> pGo) { return pGo->m_Name == name; }) };

	return it == std::end(m_Objects) ? nullptr : *it;
}

std::vector<std::shared_ptr<ngenius::GameObject>> ngenius::Scene::GetAllGameObjectsWithName(const std::string& name) const
{
	std::vector<std::shared_ptr<GameObject>> result;
	std::copy_if(std::begin(m_Objects), std::end(m_Objects), std::back_inserter(result)
		, [&name](std::shared_ptr<GameObject> pGo) { return pGo->m_Name == name; });

	return result;
}

std::shared_ptr<ngenius::GameObject> ngenius::Scene::GetGameObjectWithTag(const std::string& tag) const
{
	const auto it{ std::find_if(std::begin(m_Objects), std::end(m_Objects), [&tag](std::shared_ptr<GameObject> pGo) { return pGo->m_Tag == tag; }) };

	return it == std::end(m_Objects) ? nullptr : *it;
}

std::vector<std::shared_ptr<ngenius::GameObject>> ngenius::Scene::GetAllGameObjectsWithTag(const std::string& tag) const
{
	std::vector<std::shared_ptr<GameObject>> result;
	std::copy_if(std::begin(m_Objects), std::end(m_Objects), std::back_inserter(result)
		, [&tag](std::shared_ptr<GameObject> pGo) { return pGo->m_Tag == tag; });

	return result;
}

