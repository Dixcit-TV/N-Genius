#pragma once
#include "IComponent.h"

namespace ngenius
{
	class GameObject final
	{
	public:

		using Components = std::vector<std::unique_ptr<IComponent>>;
		
		explicit GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename COMPONENT_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, COMPONENT_TYPE>>>
		void AddComponent(std::unique_ptr<COMPONENT_TYPE>&& pNewComponent)
		{
			m_ComponentPtrs.push_back(pNewComponent);
		}

		template<typename COMPONENT_TYPE, typename... ARG_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, COMPONENT_TYPE>>>
		void AddComponent(ARG_TYPE&&... arguments)
		{
			m_ComponentPtrs.push_back(std::make_unique<COMPONENT_TYPE>(std::forward<ARG_TYPE>(arguments)...));
		}

		template<typename COMPONENT_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, COMPONENT_TYPE>>>
		const COMPONENT_TYPE& GetComponent() const
		{
			auto it{ std::find_if(std::cbegin(m_ComponentPtrs), std::cend(m_ComponentPtrs), [](const std::unique_ptr<IComponent>& c) { return dynamic_cast<COMPONENT_TYPE*>(c.get()); }) };

			if (it == std::cend(m_ComponentPtrs))
				throw std::runtime_error("Could not find component");
			
			return *static_cast<COMPONENT_TYPE*>(it->get());
		}

		const Components& GetComponents() const { return m_ComponentPtrs; };
		Components& GetComponents() { return m_ComponentPtrs; };
	
	private:
		Components m_ComponentPtrs;
	};
}
