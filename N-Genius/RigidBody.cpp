#include "RigidBody.h"

ngenius::RigidBody::RigidBody(float aabbWidth, float aabbHeight, const glm::vec2& centerOffset)
	: m_Center(centerOffset)
	, m_Width(aabbWidth)
	, m_Height(aabbHeight)
	, m_IsTrigger(true)
{}