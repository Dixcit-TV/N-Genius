#include "SceneManager.h"
#include <iostream>

#include "Scene.h"

void ngenius::SceneManager::AddScene(std::shared_ptr<Scene> pNewScene)
{
	assert(std::find_if(std::cbegin(m_Scenes), std::cend(m_Scenes), [pNewScene](const auto pScene)
		{
			return pScene->GetName() == pNewScene->GetName();
		}) == std::cend(m_Scenes) && "A scene already exists with the name !");

	m_Scenes.push_back(pNewScene);
	pNewScene->m_OnSceneInitialise.Invoke(pNewScene);
	pNewScene->m_Initalized = true;
}

void ngenius::SceneManager::SetCurrentScene(int id)
{
	if (id < 0 || id >= static_cast<int>(m_Scenes.size()))
	{
		std::cout << "Scene " + std::to_string(id) + " does not exist. Current scene was not changed!" << std::endl;
		return;
	}

	if (m_CurrentSceneId >= 0 && m_CurrentSceneId < static_cast<int>(m_Scenes.size()))
		m_Scenes[m_CurrentSceneId]->m_OnSceneDeactivate.Invoke(m_Scenes[m_CurrentSceneId]);
	
	m_CurrentSceneId = id;
	m_Scenes[m_CurrentSceneId]->m_OnSceneActivate.Invoke(m_Scenes[m_CurrentSceneId]);
}

void ngenius::SceneManager::ReloadCurrentScene()
{
	assert(m_CurrentSceneId >= 0 && m_CurrentSceneId < static_cast<int>(m_Scenes.size()));

	auto currentScene{ m_Scenes[m_CurrentSceneId] };
	currentScene->Clear();
	currentScene->m_OnSceneInitialise.Invoke(currentScene);
	currentScene->m_Initalized = true;
	currentScene->m_OnSceneActivate.Invoke(currentScene);
}

void ngenius::SceneManager::SetCurrentScene(const std::string& sceneName)
{
	auto sceneIt = std::find_if(std::cbegin(m_Scenes), std::cend(m_Scenes), [&sceneName](const auto pScene)
		{
			return pScene->GetName() == sceneName;
		});

	SetCurrentScene(static_cast<int>(std::distance(std::cbegin(m_Scenes), sceneIt)));
}

void ngenius::SceneManager::Update()
{
	assert(m_CurrentSceneId >= 0 && m_CurrentSceneId < static_cast<int>(m_Scenes.size()));
	
	m_Scenes[m_CurrentSceneId]->Update();
}

void ngenius::SceneManager::Render()
{
	assert(m_CurrentSceneId >= 0 && m_CurrentSceneId < static_cast<int>(m_Scenes.size()));
	
	m_Scenes[m_CurrentSceneId]->Render();
}