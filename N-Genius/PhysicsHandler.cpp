#include "PhysicsHandler.h"

#include "Helpers.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "Renderer.h"

void ngenius::PhysicsHandler::RegisterRigidBody(std::shared_ptr<GameObject> pGameObject)
{
	auto body{ pGameObject->GetComponent<RigidBody>() };
	if (body)
		m_pBodies.push_back(body);
}

void ngenius::PhysicsHandler::UnregisterRigidBody(std::shared_ptr<GameObject> pGameObject)
{
	auto body{ pGameObject->GetComponent<RigidBody>() };
	if (body)
		m_pBodies.erase(std::remove(std::begin(m_pBodies), std::end(m_pBodies), body), std::end(m_pBodies));
}

void ngenius::PhysicsHandler::Update()
{
	const size_t bodyCount{ m_pBodies.size() };
	for (size_t idx1{}; idx1 < bodyCount; ++idx1)
	{
		const RigidBody& body1{ *m_pBodies[idx1] };
		auto go1{ body1.GetGameObject() };
		if (!go1->IsEnabled())
			continue;
		
		glm::vec2 bodyCenter1{ go1->GetTransform().GetPosition() + body1.m_Center };
		
		for (size_t idx2{ idx1 + 1 }; idx2 < bodyCount; ++idx2)
		{
			const RigidBody& body2{ *m_pBodies[idx2] };
			auto go2{ body2.GetGameObject() };
			if (!go2->IsEnabled())
				continue;

			if (body1.m_IsTrigger || body2.m_IsTrigger)
			{	
				glm::vec2 bodyCenter2{ go2->GetTransform().GetPosition() + body2.m_Center };
				const bool overlap{ Helpers::AabbOverlapCheck(bodyCenter1, body1.m_Width, body1.m_Height
					, bodyCenter2, body2.m_Width, body2.m_Height) };

				if (overlap)
				{
					body1.m_OnOverlap.Invoke(m_pBodies[idx1], m_pBodies[idx2]);
					body2.m_OnOverlap.Invoke(m_pBodies[idx2], m_pBodies[idx1]);
				}
			}
		}
	}
}

void ngenius::PhysicsHandler::Render() const
{
	//Renderer& renderer{ Renderer::GetInstance() };
	//for (const auto& rb : m_pBodies)
	//{
	//	glm::vec2 bodyCenter{ rb->GetGameObject()->GetTransform().GetPosition() + rb->m_Center };
	//	renderer.DrawRect(bodyCenter, rb->m_Width, rb->m_Height, SDL_Color{});
	//}
}
