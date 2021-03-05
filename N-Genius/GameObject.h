#pragma once
#include "IComponent.h"
#include "TransformComponent.h"
#include <vector>
#include <memory>

namespace ngenius
{
	class GameObject final
	{
	public:

		using Components = std::vector<std::shared_ptr<IComponent>>;
		
		explicit GameObject(const std::shared_ptr<TransformComponent>& pTransform = std::make_shared<TransformComponent>());
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename COMPONENT_TYPE, typename... ARG_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, COMPONENT_TYPE> && !std::is_same_v<COMPONENT_TYPE, TransformComponent>>>
		std::shared_ptr<COMPONENT_TYPE> AddComponent(ARG_TYPE&&... arguments)
		{
			auto newComp{ std::make_shared<COMPONENT_TYPE>(std::forward<ARG_TYPE>(arguments)...) };
			newComp->SetParentGo(this);
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

		std::shared_ptr<TransformComponent> GetTransform() const { return m_pTransform; }

		void Delete() { m_MarkedForDeletion = true; };

		bool IsMarkedForDeleteion() const { return m_MarkedForDeletion; };
	
	private:
		Components m_ComponentPtrs;
		std::shared_ptr<TransformComponent> m_pTransform;
		bool m_MarkedForDeletion;
	};
}
