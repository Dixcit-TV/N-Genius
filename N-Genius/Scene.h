#pragma once
#include "SceneManager.h"

namespace ngenius
{
	class GameObject;
	class Scene : public std::enable_shared_from_this<Scene>
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<GameObject>& object);

		void Update();
		void Render() const;
		const std::string& GetName() const { return m_Name; }

		std::shared_ptr<GameObject> GetGameObjectWithName(const std::string& name) const;
		std::vector<std::shared_ptr<GameObject>> GetAllGameObjectsWithName(const std::string& name) const;

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};
	};

}
