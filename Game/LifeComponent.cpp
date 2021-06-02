#include "LifeComponent.h"

LifeComponent::LifeComponent(int lifeCount)
	: IComponent()
	, m_OnHealthDepletedEvent()
	, m_OnHealthChangeEvent()
	, m_LifeCount(lifeCount)
{}

void LifeComponent::ApplyDamage(int damageAmount)
{
	int prevLifeCount{ m_LifeCount };
	m_LifeCount = glm::clamp(m_LifeCount - damageAmount, 0, damageAmount);

	m_OnHealthChangeEvent.Invoke(prevLifeCount, m_LifeCount);
	if (m_LifeCount == 0 && prevLifeCount > m_LifeCount)
		m_OnHealthDepletedEvent.Invoke();
}