#pragma once
#include <iostream>

#include "GameObject.h"
#include "Input.h"
#include "PhysicsComponent.h"

namespace ngenius
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		virtual void Execute(const InputData& inputData) = 0;
	protected:
		explicit ICommand(std::shared_ptr<GameObject> pTargetObject) : m_pGameObject(std::move(pTargetObject)) {}

		std::shared_ptr<GameObject> m_pGameObject;
	};

	class MoveCommand final : public ICommand
	{
	public:
		explicit MoveCommand(std::shared_ptr<GameObject> pTargetObject) : ICommand(std::move(pTargetObject)){}

		void Execute(const InputData& inputData) override
		{
			auto pos{ m_pGameObject->GetTransform().GetPosition() };
			auto newPos{ pos + inputData.value };
			m_pGameObject->GetTransform().SetPosition(newPos.x, newPos.y);
		}
	};

	class KillPlayerCommand final : public ICommand
	{
	public:
		explicit KillPlayerCommand(std::shared_ptr<GameObject> pTargetObject) : ICommand(std::move(pTargetObject)) {}

		void Execute(const InputData& inputData) override
		{
			if (inputData.state == InputState::PRESSED)
			{
				auto physxComp{ m_pGameObject->GetComponent<PhysicsComponent>() };
				if (physxComp)
					physxComp->SimulateOnCollision(MessageType::PLAYER_HIT);
			}
		}
	};

	class JumpCommand final : public ICommand
	{
	public:
		explicit JumpCommand(std::shared_ptr<GameObject> pTargetObject) : ICommand(std::move(pTargetObject)) {}

		void Execute(const InputData& inputData) override
		{
			if (inputData.state == InputState::PRESSED)
			{
				auto physxComp{ m_pGameObject->GetComponent<PhysicsComponent>() };
				if (physxComp)
					physxComp->SimulateOnCollision(MessageType::BLOCK_HIT);
			}
		}
	};

	class AttackCommand final : public ICommand
	{
	public:
		explicit AttackCommand(std::shared_ptr<GameObject> pTargetObject) : ICommand(std::move(pTargetObject)) {}

		void Execute(const InputData& inputData) override
		{
			if (inputData.state == InputState::PRESSED)
			{
				auto physxComp{ m_pGameObject->GetComponent<PhysicsComponent>() };
				if (physxComp)
					physxComp->SimulateOnCollision(MessageType::COILY_HIT);
			}
		}
	};

	class CatchCommand final : public ICommand
	{
	public:
		explicit CatchCommand(std::shared_ptr<GameObject> pTargetObject) : ICommand(std::move(pTargetObject)) {}

		void Execute(const InputData& inputData) override
		{
			if (inputData.state == InputState::PRESSED)
			{
				auto physxComp{ m_pGameObject->GetComponent<PhysicsComponent>() };
				if (physxComp)
					physxComp->SimulateOnCollision(MessageType::SLICK_SAM_CAUGHT);
			}
		}
	};

}
