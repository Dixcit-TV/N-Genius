#include "ScoreComponent.h"

void ScoreComponent::OnNotify(Event event, void*)
{
	switch (event)
	{
	case Event::BLOCK_HIT:
		m_Score += 30;
		break;
	case Event::COILY_HIT:
		m_Score += 500;
		break;
	case Event::REMAINING_DISKS:
		m_Score += 50;
		break;
	case Event::SLICK_SAM_CAUGHT:
		m_Score += 300;
		break;
	default: break;
	}
}