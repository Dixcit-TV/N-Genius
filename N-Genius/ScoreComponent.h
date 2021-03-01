#pragma once
#include "IComponent.h"
#include "IObserver.h"

class ScoreComponent : public ngenius::IComponent, public ngenius::IObserver
{
public:
	explicit ScoreComponent() : m_Score{} {}
	~ScoreComponent() override = default;
	ScoreComponent(const ScoreComponent& rhs) = delete;
	ScoreComponent(ScoreComponent&& rhs) noexcept = delete;
	ScoreComponent& operator=(const ScoreComponent& rhs) = delete;
	ScoreComponent& operator=(ScoreComponent&& rhs) noexcept = delete;

	void OnNotify(Event event, void* pPayload) override;

	int GetScore() const { return m_Score; }

private:
	int m_Score;
};

