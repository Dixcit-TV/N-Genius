#include "PCH.h"
#include "GameObject.h"

//GameObject as a TransformComponent by default
//TransformComponent is unique for each game object, more cannot be added
ngenius::GameObject::GameObject(const std::shared_ptr<TransformComponent>& pTransform)
	: m_ComponentPtrs{}
	, m_pTransform{ pTransform }
	, m_MarkedForDeletion{ false }
{
	m_ComponentPtrs.push_back(m_pTransform);
}