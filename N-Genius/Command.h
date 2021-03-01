#pragma once
#include <iostream>

#include "GameObject.h"
#include "Input.h"

namespace ngenius
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		virtual void Execute(const InputData& inputData) = 0;
	protected:
		explicit ICommand(std::shared_ptr<GameObject> pTargetObject) : m_pGameObject(pTargetObject) {}

		std::shared_ptr<GameObject> m_pGameObject;
	};

	class MoveCommand final : public ICommand
	{
	public:
		explicit MoveCommand(std::shared_ptr<GameObject> pTargetObject) : ICommand(pTargetObject){}

		void Execute(const InputData& inputData) override
		{
			auto pos{ m_pGameObject->GetTransform()->GetPosition() };
			auto newPos{ pos + inputData.value };
			m_pGameObject->GetTransform()->SetPosition(newPos.x, newPos.y);
		}
	};

}