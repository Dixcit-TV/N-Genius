#pragma once
#include <memory>
#include <string>
#include <vector>

namespace ngenius
{
	class GameObject;
	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		void Add(const std::shared_ptr<GameObject>& object);

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
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};
	};

}
