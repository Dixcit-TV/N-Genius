#pragma once
#include <memory>

namespace ngenius {
	class GameObject;

	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		virtual void Update() {}
		virtual void Render() const {}

		void SetParentGo(std::weak_ptr<GameObject> pParent) { m_pGameObject = pParent; }

	protected:
		explicit IComponent() = default;
		IComponent(const IComponent& rhs) = default;
		IComponent(IComponent&& rhs) noexcept = default;
		IComponent& operator=(const IComponent& rhs) = default;
		IComponent& operator=(IComponent&& rhs) noexcept = default;

		std::weak_ptr<GameObject> m_pGameObject;
	};
}

