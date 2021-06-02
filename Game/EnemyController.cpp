#include "EnemyController.h"
#include <Command.h>

EnemyController::EnemyController(float moveSpeed, CellFace face)
	: CharacterController(moveSpeed, face)
	, m_pMoveCommands()
{}

EnemyController::~EnemyController()
{
	for (const auto& commandPair : m_pMoveCommands)
		delete commandPair.second;
}

void EnemyController::Update()
{
	if (m_State == CharacterState::IDLE)
	{
		TriggerNextMovement();
	}

	CharacterController::Update();
}

void EnemyController::TriggerNextMovement() const
{
	const int moveCount{ static_cast<int>(m_pMoveCommands.size()) };
	if (moveCount == 0)
		return;
	
	const int nextMove{ rand() % moveCount };
	auto nextMoveIt{ std::begin(m_pMoveCommands) };
	std::advance(nextMoveIt, nextMove);
	nextMoveIt->second->Execute(ngenius::InputData{ngenius::InputState::PRESSED});
}

void EnemyController::RegisterMoveCommand(Direction direction, ngenius::ICommand* pCommand)
{
	if (!m_pMoveCommands.try_emplace(direction, pCommand).second)
	{
		std::cout << "Could not add Move command !" << std::endl;
	} 
}