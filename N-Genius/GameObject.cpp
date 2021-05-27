#include "PCH.h"
#include "GameObject.h"

ngenius::GameObject::GameObject(const Transform& transform, const std::string& name)
	: m_ComponentPtrs{ Components() }
	, m_Transform{ transform }
	, m_Name{ name }
	, m_Tag{ "" }
	, m_pParentScene{ }
	, m_MarkedForDeletion{ false }
{}
