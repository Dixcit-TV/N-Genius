#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ngenius
{
	class GameObject;
	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		void Add(const std::shared_ptr<GameObject>& object, bool force = false);
		void Remove(const std::shared_ptr<GameObject>& object, bool force = false);

		void Update();
		void Render() const;
		const std::string& GetName() const { return m_Name; }

		std::shared_ptr<GameObject> GetGameObjectWithName(const std::string& name) const;
		std::vector<std::shared_ptr<GameObject>> GetAllGameObjectsWithName(const std::string& name) const;

		virtual ~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
		
		virtual void Initialise() {}
		virtual void Activate() {}
		virtual void Deactivate() {}

	protected:
		friend class SceneManager;
		
		explicit Scene(const std::string& name);

		void RootInitialise()
		{
			Initialise();
			m_Initalized = true;
		}

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects;
		std::vector<std::function<void()>> m_SceneQueue;
		bool m_Initalized;
	};

}
