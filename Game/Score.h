#pragma once
#include <IComponent.h>
#include "Delegate.h"
#include "Enums.h"

class Score final : public ngenius::IComponent
{
public:
	explicit Score();
	~Score() override = default;

	Score(const Score& rhs) = default;
	Score(Score&& rhs) noexcept = default;
	Score& operator=(const Score& rhs) = default;
	Score& operator=(Score&& rhs) noexcept = default;

	void UpdateScore(ScoreEventType scoreEvent);
	void RegisterScoreUpdateEvent(const std::string& evtName, const std::function<void(std::string)>& callback) { m_ScoreUpdateEvent.Register(evtName, callback); }

	int GetScore() const { return m_Score; }
	void SetScore(int newScore)
	{
		m_Score = newScore;
		m_ScoreUpdateEvent.Invoke("Score: " + std::to_string(m_Score));
	}

private:
	ngenius::Event<std::string> m_ScoreUpdateEvent;
	std::map<ScoreEventType, int> m_ScoreRules;
	int m_Score;

};

