#pragma once
#include "Transform.h"
#include <vector>
#include <memory>
#include <string>

#include "Scene.h"

namespace ngenius
{
	class Scene;
	class IComponent;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
		friend class Scene;
	public:

		using Components = std::vector<std::shared_ptr<IComponent>>;
		
		explicit GameObject(const Transform& transform = Transform(), const std::string& name = "GameObject", const std::string& tag = "");
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename COMPONENT_TYPE, typename... ARG_TYPE, typename = std::enable_if_t<std::is_base_of_v<IComponent, COMPONENT_TYPE>>>
		std::shared_ptr<COMPONENT_TYPE> AddComponent(ARG_TYPE&&... arguments)
		{
			auto newComp{ std::make_shared<COMPONENT_TYPE>(std::forward<ARG_TYPE>(arguments)...) };
			newComp->SetParentGo(weak_from_this());
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

		std::shared_ptr<GameObject> GetGameObjectWithName(const std::string& name) const { return m_pParentScene.lock()->GetGameObjectWithName(name); }
		std::vector<std::shared_ptr<GameObject>> GetAllGameObjectsWithName(const std::string& name) const { return m_pParentScene.lock()->GetAllGameObjectsWithName(name); }
		std::shared_ptr<GameObject> GetGameObjectWithTag(const std::string& tag) const { return m_pParentScene.lock()->GetGameObjectWithTag(tag); }
		std::vector<std::shared_ptr<GameObject>> GetAllGameObjectsWithTag(const std::string& tag) const { return m_pParentScene.lock()->GetAllGameObjectsWithTag(tag); }

		std::shared_ptr<Scene> GetScene() const { return m_pParentScene.lock(); }

		const std::string& GetName() const { return m_Name; }
		const std::string& GetTag() const { return m_Tag; }
		void SetTag(const std::string& newTag) { m_Tag = newTag; }

		void SetEnable(bool enable) { m_IsEnabled = enable; }
		bool IsEnabled() const { return m_IsEnabled; }

		void Delete() { m_pParentScene.lock()->Remove(shared_from_this()); }
	
	private:
		Components m_ComponentPtrs;
		Transform m_Transform;
		std::string m_Name;
		std::string m_Tag;
		std::weak_ptr<Scene> m_pParentScene;
		bool m_IsEnabled;
	};
}
