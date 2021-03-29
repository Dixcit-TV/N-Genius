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
}
