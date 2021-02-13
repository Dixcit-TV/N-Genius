#pragma once
#include "Singleton.h"

namespace ngenius
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager() override = default;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;
		
		Scene& CreateScene(const std::string& name);

		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		explicit SceneManager() = default;
		
		std::vector<std::shared_ptr<Scene>> m_Scenes;
	};
}
