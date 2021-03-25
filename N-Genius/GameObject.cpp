#include "PCH.h"
#include "GameObject.h"

ngenius::GameObject::GameObject(const Transform& transform)
	: m_ComponentPtrs{}
	, m_Transform{ transform }
	, m_MarkedForDeletion{ false }
{}