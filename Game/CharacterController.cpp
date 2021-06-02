#include "CharacterController.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <gtx/norm.hpp>
#include <gtx/compatibility.hpp>
#pragma warning(pop)

#include "TimeSettings.h"

CharacterController::CharacterController(float moveSpeed, CellFace face)
	: IComponent()
	, m_TargetPosition()
	, m_OnEndMovementEvent()
	, m_MovementSpeed(moveSpeed)
	, m_State(CharacterState::IDLE)
	, m_Face(face)
	, m_IsJumpingOut(false)
{}

void CharacterController::Update()
{
	const float distEpsilon{ 0.1f };
	ngenius::Transform& transform{ GetTransform() };
	const glm::vec2& position{ transform.GetPosition() };

	if (m_State == CharacterState::MOVING && length2(position - m_TargetPosition) > distEpsilon)
	{
		transform.SetPosition(lerp(position, m_TargetPosition, m_MovementSpeed * ngenius::TimeSettings::GetInstance().GetElapsed()));
	}
	else if (m_State == CharacterState::MOVING)
	{
		if (m_IsJumpingOut)
			m_OnJumpOutMovementEvent.Invoke();
		
		m_State = CharacterState::IDLE;
		m_OnEndMovementEvent.Invoke(position);
		m_IsJumpingOut = false;
	}
}