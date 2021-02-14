#include "PCH.h"
#include "TransformComponent.h"

void ngenius::TransformComponent::Receive([[maybe_unused]] eComponentMessage message, [[maybe_unused]] const std::shared_ptr<GameObject>& pParentGo)
{
	//switch (message)
	//{
	//default: break;
	//}
}

void ngenius::TransformComponent::SetPosition(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;
}
