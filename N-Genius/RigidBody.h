#pragma once
#include "Delegate.h"
#include "IComponent.h"

namespace ngenius
{
	class RigidBody final : public IComponent
	{
		friend class PhysicsHandler;
	public:
		explicit RigidBody(float aabbWidth, float aabbHeight, const glm::vec2& centerOffset = { 0.f, 0.f });
		~RigidBody() override = default;

		RigidBody(const RigidBody& rhs) = delete;
		RigidBody(RigidBody&& rhs) noexcept = delete;
		RigidBody& operator=(const RigidBody& rhs) = delete;
		RigidBody& operator=(RigidBody&& rhs) noexcept = delete;

		void RegisterOnOverlapEvent(const std::string& evtName, const std::function<void(std::shared_ptr<RigidBody>, std::shared_ptr<RigidBody>)>& callback) { m_OnOverlap.Register(evtName, callback); }
		bool IsTrigger() const { return m_IsTrigger; }

	private:
		glm::vec2 m_Center;
		Event<std::shared_ptr<RigidBody>, std::shared_ptr<RigidBody>> m_OnOverlap;
		float m_Width;
		float m_Height;
		bool m_IsTrigger;
	};

}
