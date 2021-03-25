#include "ScoreComponent.h"

void ScoreComponent::OnNotify(MessageType event, void*)
{
	switch (event)
	{
	case MessageType::BLOCK_HIT:
		m_Score += 30;
		break;
	case MessageType::COILY_HIT:
		m_Score += 500;
		break;
	case MessageType::REMAINING_DISKS:
		m_Score += 50;
		break;
	case MessageType::SLICK_SAM_CAUGHT:
		m_Score += 300;
		break;
	default: break;
	}
}