#pragma once
#include "IComponent.h"
#include "Transform.h"
#include <vector>
#include <memory>

namespace ngenius
{
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:

		using Components = std::vector<std::shared_ptr<IComponent>>;
		
		explicit GameObject(const Transform& transform = Transform());
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename COMPONENT_TYPE, typename... ARG_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, COMPONENT_TYPE>>>
		std::shared_ptr<COMPONENT_TYPE> AddComponent(ARG_TYPE&&... arguments)
		{
			auto newComp{ std::make_shared<COMPONENT_TYPE>(std::forward<ARG_TYPE>(arguments)...) };
			newComp->SetParentGo(shared_from_this());
			m_ComponentPtrs.push_back(newComp);
			return newComp;
		}

		template<typename COMPONENT_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, COMPONENT_TYPE>>>
		std::shared_ptr<COMPONENT_TYPE> GetComponent() const
		{
			auto it{ std::find_if(std::cbegin(m_ComponentPtrs), std::cend(m_ComponentPtrs), [](const Components::value_type& c) { return std::dynamic_pointer_cast<COMPONENT_TYPE>(c); }) };

			return it != std::cend(m_ComponentPtrs) ? std::static_pointer_cast<COMPONENT_TYPE>(*it) : nullptr;
		}

		const Components& GetAllComponents() const { return m_ComponentPtrs; }
		Components& GetAllComponents() { return m_ComponentPtrs; }

		const Transform& GetTransform() const { return m_Transform; }
		Transform& GetTransform() { return m_Transform; }

		void Delete() { m_MarkedForDeletion = true; };

		bool IsMarkedForDeletion() const { return m_MarkedForDeletion; };
	
	private:
		Components m_ComponentPtrs;
		Transform m_Transform;
		bool m_MarkedForDeletion;
	};
}
