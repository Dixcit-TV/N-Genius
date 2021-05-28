#include "Stats.h"
#include "Pyramid.h"

Stats::Stats(int startLifeCount)
	: IComponent()
	, m_ScoreUpdateEvent()
	, m_ScoreRules()
	, m_Score(0)
	, m_LiveCount(startLifeCount)
{
	m_ScoreRules[ScoreEventType::COLOR_CHANGE] = 25;
	m_ScoreRules[ScoreEventType::COILY_DEFEAT] = 500;
	m_ScoreRules[ScoreEventType::REMAINING_DISK] = 50;
	m_ScoreRules[ScoreEventType::SLICK_SAM_CAUGHT] = 300;
}

void Stats::UpdateScore(ScoreEventType scoreEvent)
{
	auto it{ m_ScoreRules.find(scoreEvent) };

	if (it != std::end(m_ScoreRules))
	{
		m_Score += it->second;
		m_ScoreUpdateEvent.Invoke("Score: " + std::to_string(m_Score));
	}
}
