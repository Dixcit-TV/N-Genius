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

	protected:		
		explicit IComponent() = default;
		IComponent(const IComponent& rhs) = default;
		IComponent(IComponent&& rhs) noexcept = default;
		IComponent& operator=(const IComponent& rhs) = default;
		IComponent& operator=(IComponent&& rhs) noexcept = default;

		GameObject* m_pGameObject;

	private:
		friend class GameObject;
		
		void SetParentGo(GameObject* pParent) { m_pGameObject = pParent; }
	};
}

