#pragma once
#include <memory>
#include <vector>

namespace ngenius
{
	class GameObject;
	class RigidBody;

	class PhysicsHandler final
	{
		friend class Scene;
	public:
		explicit PhysicsHandler() = default;
		PhysicsHandler(const PhysicsHandler& rhs) = delete;
		PhysicsHandler(PhysicsHandler&& rhs) noexcept = delete;
		PhysicsHandler& operator=(const PhysicsHandler& rhs) = delete;
		PhysicsHandler& operator=(PhysicsHandler&& rhs) noexcept = delete;
		~PhysicsHandler() = default;

	private:
		void RegisterRigidBody(std::shared_ptr<GameObject> pGameObject);
		void UnregisterRigidBody(std::shared_ptr<GameObject> pGameObject);
		void Update();
		void Render() const;
		void Clear() { m_pBodies.clear(); }
		
		std::vector<std::shared_ptr<RigidBody>> m_pBodies;
	};
}

