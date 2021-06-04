#include "PCH.h"
#include "GameObject.h"

ngenius::GameObject::GameObject(const Transform& transform, const std::string& name, const std::string& tag)
	: m_ComponentPtrs{ }
	, m_Transform{ transform }
	, m_Name{ name }
	, m_Tag{ tag }
	, m_pParentScene{ }
{}
