#pragma once
#include <IComponent.h>
#include "Delegate.h"
#include "Enums.h"

class Stats final : public ngenius::IComponent
{
public:
	explicit Stats(int startLifeCount);
	~Stats() override = default;

	Stats(const Stats& rhs) = default;
	Stats(Stats&& rhs) noexcept = default;
	Stats& operator=(const Stats& rhs) = default;
	Stats& operator=(Stats&& rhs) noexcept = default;

	void UpdateScore(ScoreEventType scoreEvent);
	void RegisterScoreUpdateEvent(const std::string& evtName, const std::function<void(std::string)>& callback) { m_ScoreUpdateEvent.Register(evtName, callback); }

private:
	ngenius::Event<std::string> m_ScoreUpdateEvent;
	std::map<ScoreEventType, int> m_ScoreRules;
	int m_Score;
	int m_LiveCount;

};

