#include "PCH.h"
#include "SceneManager.h"
#include "Scene.h"

void ngenius::SceneManager::SetCurrentScene(int id)
{
	if (id < 0 || id >= int(m_Scenes.size()))
	{
		throw std::runtime_error("Scene" + std::to_string(id) + " does not exist.");
	}

	m_CurrentSceneId = id;
}

void ngenius::SceneManager::SetCurrentScene(const std::string& sceneName)
{
	auto sceneIt = std::find_if(std::cbegin(m_Scenes), std::cend(m_Scenes), [&sceneName](const auto pScene)
		{
			return pScene->GetName() == sceneName;
		});

	SetCurrentScene(int(std::distance(std::cbegin(m_Scenes), sceneIt)));
}

void ngenius::SceneManager::Update()
{
	m_Scenes[m_CurrentSceneId]->Update();
}

void ngenius::SceneManager::Render()
{
	m_Scenes[m_CurrentSceneId]->Render();
}

ngenius::Scene& ngenius::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}
