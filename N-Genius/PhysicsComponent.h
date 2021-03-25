#pragma once
#include <memory>
#include "IComponent.h"
#include "Subject.h"

namespace ngenius {
	class IObserver;
}

class PhysicsComponent final : public ngenius::IComponent
{
public:
	explicit PhysicsComponent();
	~PhysicsComponent() override = default;
	PhysicsComponent(const PhysicsComponent& rhs) = delete;
	PhysicsComponent(PhysicsComponent&& rhs) noexcept = delete;
	PhysicsComponent& operator=(const PhysicsComponent& rhs) = delete;
	PhysicsComponent& operator=(PhysicsComponent&& rhs) noexcept = delete;

	void SimulateOnCollision(MessageType event) const;

	void AddCollisionListener(std::weak_ptr<ngenius::IObserver> pObserver) const;

private:
	std::unique_ptr<ngenius::Subject> m_OnCollisionSubject;
};

