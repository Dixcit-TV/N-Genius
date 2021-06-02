#include "LifeComponent.h"

#include <iostream>

LifeComponent::LifeComponent(int lifeCount)
	: IComponent()
	, m_OnHealthDepletedEvent()
	, m_OnHealthChangeEvent()
	, m_LifeCount(lifeCount)
{}

void LifeComponent::ApplyDamage(int damageAmount)
{
	const int prevLifeCount{ m_LifeCount };
	m_LifeCount -= damageAmount;
	m_LifeCount = m_LifeCount < 0 ? 0 : m_LifeCount;

	if (prevLifeCount != m_LifeCount)
	{
		m_OnHealthChangeEvent.Invoke(prevLifeCount, m_LifeCount);

		if (m_LifeCount == 0)
			m_OnHealthDepletedEvent.Invoke();
	}
}