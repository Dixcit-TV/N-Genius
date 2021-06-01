#pragma once
#include <map>

#include "Enums.h"
#include "CharacterController.h"

namespace ngenius {
	class ICommand;
}

class EnemyController final : public CharacterController
{
public:
	explicit EnemyController(float moveSpeed, CellFace face);
	virtual ~EnemyController() override;

	EnemyController(const EnemyController& rhs) = delete;
	EnemyController(EnemyController&& rhs) noexcept = delete;
	EnemyController& operator=(const EnemyController& rhs) = delete;
	EnemyController& operator=(EnemyController&& rhs) noexcept = delete;

	void Update() override;

	void RegisterMoveCommand(Direction, ngenius::ICommand* pCommand);
	void TriggerNextMovement() const;

private:
	std::map<Direction, ngenius::ICommand*> m_pMoveCommands;
};

