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
		explicit Transform() = default;
		explicit Transform(const glm::vec2& position, const glm::vec2& scale = {1.f, 1.f}, float rotation = 0.f)
			: m_Position(position), m_Scale(scale), m_Rotation(rotation) {};

		const glm::vec2& GetPosition() const;
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec2& position);

		float GetRotation() const;
		void SetRotation(float angle, bool isDegree = true);

		const glm::vec2& GetScale() const;
		void SetScale(float x, float y);
		void SetScale(const glm::vec2& scale);
	
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Scale;
		float m_Rotation;
	};
}
