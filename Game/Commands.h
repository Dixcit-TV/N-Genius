#pragma once
#include "HelperFunctions.h"
#include "Enums.h"
#include <Command.h>
#include "Pyramid.h"
#include "Qbert.h"

class MoveCommand final : public ngenius::ICommand
{
public:
	explicit MoveCommand(std::shared_ptr<ngenius::GameObject> pTargetObject, Direction direction) : ICommand(std::move(pTargetObject)), m_Direction(direction) {}

	void Execute(const ngenius::InputData& inputData) override
	{
		if (inputData.state == ngenius::InputState::PRESSED)
		{
			auto characterComp{ m_pGameObject->GetComponent<Qbert>() };
			auto pyramid{ m_pGameObject->GetGameObjectWithName("Pyramid") };
			std::shared_ptr<Pyramid> pyramidComp{ };
			
			if (pyramid 
				&& characterComp 
				&& (pyramidComp = pyramid->GetComponent<Pyramid>())
				&& characterComp->GetState() == QBertState::IDLE)
			{
				const glm::vec2& goPos{ m_pGameObject->GetTransform().GetPosition() };
				const glm::vec2 dir{ Helpers::EnumToDirection(m_Direction) };
				const glm::vec2 targetPos{ pyramidComp->GetTargetPosition(goPos, dir) };
				characterComp->SetTargetPosition(targetPos);
				characterComp->SetState(QBertState::MOVING);
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
