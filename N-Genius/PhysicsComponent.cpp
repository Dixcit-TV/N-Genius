#include "PhysicsComponent.h"
#include "Subject.h"

PhysicsComponent::PhysicsComponent()
	: IComponent()
	, m_OnCollisionSubject(std::make_unique<ngenius::Subject>())
{}

void PhysicsComponent::SimulateOnCollision(MessageType event) const
{
	m_OnCollisionSubject->Notify(event, nullptr);
}

void PhysicsComponent::AddCollisionListener(std::weak_ptr<ngenius::IObserver> pObserver) const
{
	m_OnCollisionSubject->AddObserver(std::move(pObserver));
}