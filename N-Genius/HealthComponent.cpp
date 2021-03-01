#include "HealthComponent.h"

HealthComponent::HealthComponent(int maxHealth)
	: IComponent(), m_HealthCount(maxHealth)
{}

void HealthComponent::OnNotify(Event event, void*)
{
	switch (event)
	{
	case Event::PLAYER_HIT:
		OnPlayerDied();
		break;
	}
}

void HealthComponent::OnPlayerDied()
{
	--m_HealthCount;
}