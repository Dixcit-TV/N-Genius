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

		std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject.lock(); }

	protected:		
		explicit IComponent() = default;
		IComponent(const IComponent& rhs) = default;
		IComponent(IComponent&& rhs) noexcept = default;
		IComponent& operator=(const IComponent& rhs) = default;
		IComponent& operator=(IComponent&& rhs) noexcept = default;

		std::weak_ptr<GameObject> m_pGameObject;

		Transform& GetTransform() { return m_pGameObject.lock()->GetTransform(); }
		const Transform& GetTransform() const { return m_pGameObject.lock()->GetTransform(); }

		template<typename COMPONENT_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, COMPONENT_TYPE>>>
		std::shared_ptr<COMPONENT_TYPE> GetComponent() const { return m_pGameObject.lock()->GetComponent<COMPONENT_TYPE>(); }

	private:
		friend class GameObject;
		
		void SetParentGo(std::weak_ptr<GameObject> pParent) { m_pGameObject = std::move(pParent); }
	};
}

