#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"

unsigned int ngenius::Scene::m_IdCounter = 0;

ngenius::Scene::Scene(const std::string& name) : m_Name(name) {}

void ngenius::Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
}

void ngenius::Scene::Update()
{
	for(auto object : m_Objects)
	{
		const GameObject::Components& components{ object->GetAllComponents() };
		for (auto comp : components)
		{
			comp->Receive(eComponentMessage::UPDATE, object);
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
			comp->Receive(eComponentMessage::RENDER, object);
		}
	}
}

