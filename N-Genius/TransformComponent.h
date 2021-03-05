#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

#include "IComponent.h"

namespace ngenius
{
	class TransformComponent final : public IComponent
	{
	public:
		explicit TransformComponent() = default;
		explicit TransformComponent(float x, float y) : IComponent(), m_Position(x, y) {}
		explicit TransformComponent(const glm::vec2& position) : IComponent(), m_Position(position) {};

		~TransformComponent() override = default;
		TransformComponent(const TransformComponent& rhs) = delete;
		TransformComponent(TransformComponent&& rhs) noexcept = delete;
		TransformComponent& operator=(const TransformComponent& rhs) = delete;
		TransformComponent& operator=(TransformComponent&& rhs) noexcept = delete;

		const glm::vec2& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y);
	
	private:
		glm::vec2 m_Position;
	};
}
