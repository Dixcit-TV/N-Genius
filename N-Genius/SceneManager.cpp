#include "PCH.h"
#include "SceneManager.h"
#include <cassert>
#include "Scene.h"

void ngenius::SceneManager::SetCurrentScene(int id)
{
	if (id < 0 || id >= static_cast<int>(m_Scenes.size()))
	{
		std::cout << "Scene " + std::to_string(id) + " does not exist. Current scene was not changed!" << std::endl;
		return;
	}

	if (m_CurrentSceneId >= 0 && m_CurrentSceneId < static_cast<int>(m_Scenes.size()))
		m_Scenes[m_CurrentSceneId]->Deactivate();
	
	m_CurrentSceneId = id;
	m_Scenes[m_CurrentSceneId]->Activate();
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