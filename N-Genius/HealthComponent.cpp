#include "HealthComponent.h"

HealthComponent::HealthComponent(int maxHealth)
	: IComponent(), m_HealthCount(maxHealth)
{}

void HealthComponent::OnNotify(MessageType event, void*)
{
	switch (event)
	{
	case MessageType::PLAYER_HIT:
		OnPlayerDied();
		break;
	}
}

void HealthComponent::OnPlayerDied()
{
	--m_HealthCount;
}