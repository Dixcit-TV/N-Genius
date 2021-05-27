#pragma once
#include <memory>
#include "GameObject.h"
#include "Transform.h"

namespace ngenius {
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

		std::weak_ptr<GameObject> m_pGameObject;

		Transform& GetTransform() { return m_pGameObject.lock()->GetTransform(); }
		const Transform& GetTransform() const { return m_pGameObject.lock()->GetTransform(); }

	private:
		friend class GameObject;
		
		void SetParentGo(std::weak_ptr<GameObject> pParent) { m_pGameObject = std::move(pParent); }
	};
}

