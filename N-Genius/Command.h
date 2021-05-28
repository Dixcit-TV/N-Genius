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
		ICommand(const ICommand&) = default;
		ICommand(ICommand&&) noexcept = default;
		ICommand& operator=(const ICommand&) = default;
		ICommand& operator=(ICommand&&) noexcept = default;
		
		virtual void Execute(const InputData& inputData) = 0;
	protected:
		explicit ICommand(std::weak_ptr<GameObject> pTargetObject) : m_pGameObject(std::move(pTargetObject)) {}

		std::weak_ptr<GameObject> m_pGameObject;
	};
}
