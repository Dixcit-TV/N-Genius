#include "PCH.h"
#include "Transform.h"

void ngenius::Transform::SetPosition(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void ngenius::Transform::SetPosition(const glm::vec2& position)
{
	SetPosition(position.x, position.y);
}

const glm::vec2& ngenius::Transform::GetPosition() const
{
	return m_Position;
}

float ngenius::Transform::GetRotation() const
{
	return m_Rotation;
}

void ngenius::Transform::SetRotation(float rad)
{
	m_Rotation = rad;
}

const glm::vec2& ngenius::Transform::GetScale() const
{
	return m_Scale;
}

void ngenius::Transform::SetScale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}

void ngenius::Transform::SetScale(const glm::vec2& scale)
{
	SetScale(scale.x, scale.y);
}