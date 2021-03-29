#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>
#pragma warning(pop)

namespace ngenius
{
	class Transform final
	{
	public:
		explicit Transform() : Transform(0.f, 0.f) {}
		explicit Transform(float x, float y) : m_Position(x, y), m_Scale(1.f, 1.f), m_Rotation(0.f) {}
		explicit Transform(const glm::vec2& position) : m_Position(position), m_Scale(1.f, 1.f), m_Rotation(0.f) {};

		const glm::vec2& GetPosition() const;
		void SetPosition(float x, float y);

		float GetRotation() const;
		void SetRotation(float rad);

		const glm::vec2& GetScale() const;
		void SetScale(float x, float y);
	
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Scale;
		float m_Rotation;
	};
}
