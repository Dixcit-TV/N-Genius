#pragma once
#include "Delegate.h"
#include "Enums.h"
#include "IComponent.h"

class Qbert final : public ngenius::IComponent
{
public:
	explicit Qbert(float moveSpeed, CellFace face);
	~Qbert() override = default;

	Qbert(const Qbert& rhs) = default;
	Qbert(Qbert&& rhs) noexcept = default;
	Qbert& operator=(const Qbert& rhs) = default;
	Qbert& operator=(Qbert&& rhs) noexcept = default;

	void Update() override;

	void RegisterEndMoveEvent(const std::string& evtName, const std::function<void(glm::vec2)>& callback) { m_OnEndMovementEvent.Register(evtName, callback); }
	void SetTargetPosition(const glm::vec2& targetPos) { m_TargetPosition = targetPos; }
	void SetState(QBertState newSTate){ m_State = newSTate; }
	QBertState GetState() const { return m_State; }
	CellFace GetCellFace() const { return m_Face; }

private:
	glm::vec2 m_TargetPosition;
	ngenius::Event<glm::vec2> m_OnEndMovementEvent;
	float m_MovementSpeed;
	QBertState m_State;
	CellFace m_Face;
};

