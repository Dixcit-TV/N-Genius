#pragma once
#include <IComponent.h>
#include <map>

#include "Enums.h"

namespace ngenius {
	class ICommand;
}

class EnemyController : public ngenius::IComponent
{
public:
	explicit EnemyController();
	virtual ~EnemyController() override;

	EnemyController(const EnemyController& rhs) = delete;
	EnemyController(EnemyController&& rhs) noexcept = delete;
	EnemyController& operator=(const EnemyController& rhs) = delete;
	EnemyController& operator=(EnemyController&& rhs) noexcept = delete;

	void RegisterMoveCommand(Direction, ngenius::ICommand* pCommand);
	void TriggerNextMovement(const glm::vec2& position) const;

private:
	std::map<Direction, ngenius::ICommand*> m_pMoveCommands;
};

