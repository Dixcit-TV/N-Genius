#pragma once

#include <cassert>

#include "Singleton.h"
#include <vector>
#include <memory>

#include "Scene.h"

namespace ngenius
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager() override = default;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;
		
		void AddScene(std::shared_ptr<Scene> pNewScene);
		void SetCurrentScene(int id);
		void SetCurrentScene(const std::string& sceneName);
		void ReloadCurrentScene();

		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		explicit SceneManager() = default;
		
		std::vector<std::shared_ptr<Scene>> m_Scenes{};
		int m_CurrentSceneId = -1;		
	};
}
