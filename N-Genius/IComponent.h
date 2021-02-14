#pragma once
#include "Enum.h"

namespace ngenius {
	class GameObject;

	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		virtual void Receive(eComponentMessage message, const std::shared_ptr<GameObject>& pParentGo) = 0;

	protected:
		explicit IComponent() = default;
		IComponent(const IComponent& rhs) = default;
		IComponent(IComponent&& rhs) noexcept = default;
		IComponent& operator=(const IComponent& rhs) = default;
		IComponent& operator=(IComponent&& rhs) noexcept = default;
	};
}

