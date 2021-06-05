#pragma once
#include "Enums.h"
#include <GameMode.h>
#include "structs.h"

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
	GameData& GetGameData() { return m_GameData; }
	const GameData& GetGameData() const { return m_GameData; }

private:
	GameData m_GameData;
	GameState m_CurrentState;
	GameType m_SelectedGameType;
	bool m_StateChanged;

	void SelectSceneFromGameType() const;
	void InitGameData();
};

