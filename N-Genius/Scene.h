#pragma once
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "Delegate.h"
#include "PhysicsHandler.h"

namespace ngenius
{
	class GameObject;
	class Scene final : public std::enable_shared_from_this<Scene>
	{
	public:
		explicit Scene(const std::string& name);
		~Scene() = default;
		Scene(const Scene & other) = delete;
		Scene(Scene && other) = delete;
		Scene& operator=(const Scene & other) = delete;
		Scene& operator=(Scene && other) = delete;
		
		void Add(const std::shared_ptr<GameObject>& object, bool force = false);
		void Remove(const std::shared_ptr<GameObject>& object, bool force = false);

		void Update();
		void Render() const;
		void Clear();
		const std::string& GetName() const { return m_Name; }

		std::shared_ptr<GameObject> GetGameObjectWithName(const std::string& name) const;
		std::vector<std::shared_ptr<GameObject>> GetAllGameObjectsWithName(const std::string& name) const;

		void RegisterInitialiseEvent(const std::string& evtName, std::function<void(std::shared_ptr<Scene>)> callback) { m_OnSceneInitialise.Register(evtName, std::move(callback)); }
		void RegisterActivateEvent(const std::string& evtName, std::function<void(std::shared_ptr<Scene>)> callback) { m_OnSceneActivate.Register(evtName, std::move(callback)); }
		void RegisterDeactivateEvent(const std::string& evtName, std::function<void(std::shared_ptr<Scene>)> callback) { m_OnSceneDeactivate.Register(evtName, std::move(callback)); }

	protected:
		friend class SceneManager;

		PhysicsHandler m_PhysxHandler;
		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects;
		std::queue<std::function<void()>> m_SceneQueue;
		Event<std::shared_ptr<Scene>> m_OnSceneInitialise;
		Event<std::shared_ptr<Scene>> m_OnSceneActivate;
		Event<std::shared_ptr<Scene>> m_OnSceneDeactivate;
		bool m_Initalized;
	};

}
