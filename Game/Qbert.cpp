#include "Qbert.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <gtx/norm.hpp>
#include <gtx/compatibility.hpp>
#pragma warning(pop)

#include "TimeSettings.h"

Qbert::Qbert(float moveSpeed)
	: IComponent()
	,  m_TargetPosition()
	, m_OnEndMovementEvent()
	, m_MovementSpeed(moveSpeed)
	, m_State(QBertState::IDLE)
{}

void Qbert::Update()
{
	const float distEpsilon{ 0.1f };
	ngenius::Transform& transform{ GetTransform() };
	const glm::vec2& position{ transform.GetPosition() };

	if (m_State == QBertState::MOVING && length2(position - m_TargetPosition) > distEpsilon)
	{
		transform.SetPosition(lerp(position, m_TargetPosition, m_MovementSpeed * ngenius::TimeSettings::GetInstance().GetElapsed()));
	}
	else if (m_State == QBertState::MOVING)
	{
		m_State = QBertState::IDLE;
		m_OnEndMovementEvent.Invoke(position);
	}
}
