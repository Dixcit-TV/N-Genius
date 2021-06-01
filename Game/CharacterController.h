#pragma once
#include "Delegate.h"
#include "Enums.h"
#include "IComponent.h"

class CharacterController : public ngenius::IComponent
{
public:
	explicit CharacterController(float moveSpeed, CellFace face);
	virtual ~CharacterController() override = default;

	CharacterController(const CharacterController& rhs) = delete;
	CharacterController(CharacterController&& rhs) noexcept = delete;
	CharacterController& operator=(const CharacterController& rhs) = delete;
	CharacterController& operator=(CharacterController&& rhs) noexcept = delete;

	virtual void Update() override;

	void RegisterEndMoveEvent(const std::string& evtName, const std::function<void(glm::vec2)>& callback) { m_OnEndMovementEvent.Register(evtName, callback); }
	void SetTargetPosition(const glm::vec2& targetPos) { m_TargetPosition = targetPos; }
	void SetState(CharacterState newSTate){ m_State = newSTate; }
	CharacterState GetState() const { return m_State; }
	CellFace GetCellFace() const { return m_Face; }

protected:
	glm::vec2 m_TargetPosition;
	ngenius::Event<glm::vec2> m_OnEndMovementEvent;
	float m_MovementSpeed;
	CharacterState m_State;
	CellFace m_Face;
};

