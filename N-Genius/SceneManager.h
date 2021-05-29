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

		template<typename SCENE_TYPE, typename = std::enable_if_t<std::is_base_of_v<Scene, SCENE_TYPE>>>
		void AddScene(const std::string& name)
		{
			assert(std::find_if(std::cbegin(m_Scenes), std::cend(m_Scenes), [&name](const auto pScene)
				{
					return pScene->GetName() == name;
				}) == std::cend(m_Scenes) && "A scene already exists with the name !");

			m_Scenes.push_back(std::make_shared<SCENE_TYPE>(name));
			m_Scenes.back()->Initialise();
		}

		void SetCurrentScene(int id);
		void SetCurrentScene(const std::string& sceneName);

		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		explicit SceneManager() = default;
		
		std::vector<std::shared_ptr<Scene>> m_Scenes{};
		int m_CurrentSceneId = -1;		
	};
}
