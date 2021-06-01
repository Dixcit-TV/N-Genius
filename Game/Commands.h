#pragma once
#include "HelperFunctions.h"
#include "Enums.h"
#include <Command.h>
#include "Pyramid.h"
#include "CharacterController.h"

class MoveCommand final : public ngenius::ICommand
{
public:
	explicit MoveCommand(std::weak_ptr<ngenius::GameObject> pTargetObject, Direction direction) : ICommand(std::move(pTargetObject)), m_Direction(direction) {}
	
	void Execute(const ngenius::InputData& inputData) override
	{
		if (inputData.state == ngenius::InputState::PRESSED)
		{
			auto characterGo{ m_pGameObject.lock() };
			auto characterComp{ characterGo->GetComponent<CharacterController>() };
			auto pyramid{ characterGo->GetGameObjectWithName("Pyramid") };
			std::shared_ptr<Pyramid> pyramidComp{ };
			
			if (pyramid 
				&& characterComp 
				&& (pyramidComp = pyramid->GetComponent<Pyramid>()))
			{
				if (characterComp->GetState() != CharacterState::IDLE)
					return;
				
				const glm::vec2& goPos{ characterGo->GetTransform().GetPosition() };
				CellFace face{ characterComp->GetCellFace() };
				const glm::vec2 dir{ Helpers::EnumToDirection(m_Direction, face) };
				const glm::vec2 targetPos{ pyramidComp->GetTargetPosition(goPos, dir, face) };
				characterComp->SetTargetPosition(targetPos);
				characterComp->SetState(CharacterState::MOVING);
			}
			else
			{
				std::cout << "pyramid, qbertComp or pyramidComp is missing !" << std::endl;
			}
		}
	}

private:
	Direction m_Direction;
};