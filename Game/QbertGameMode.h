#pragma once
#include "Enums.h"
#include <GameMode.h>

class QbertGameMode final : public ngenius::GameMode
{
public:
	explicit QbertGameMode();
	~QbertGameMode() override = default;
	QbertGameMode(const QbertGameMode& other) = default;
	QbertGameMode(QbertGameMode&& other) = default;
	QbertGameMode& operator=(const QbertGameMode& other) = default;
	QbertGameMode& operator=(QbertGameMode&& other) = default;

	void Update() override;
	void SetState(GameState newState);
	void SetGameType(GameType newSelectedGameType) { m_SelectedGameType = newSelectedGameType; }

private:
	GameState m_CurrentState;
	GameType m_SelectedGameType;

	void SelectSceneFromGameType() const;
};

