#pragma once
#include <IComponent.h>

#include "Enums.h"

class EnemySpawner final : public ngenius::IComponent, public std::enable_shared_from_this<EnemySpawner>
{
	struct SpawnTimer
	{
		float duration;
		float timer;
		EnemyType spawnType;
	};

public:
	explicit EnemySpawner();
	~EnemySpawner() override = default;

	EnemySpawner(const EnemySpawner& rhs) = delete;
	EnemySpawner(EnemySpawner&& rhs) noexcept = delete;
	EnemySpawner& operator=(const EnemySpawner& rhs) = delete;
	EnemySpawner& operator=(EnemySpawner&& rhs) noexcept = delete;

	void Update() override;

	void QueueSpawn(EnemyType eType);

private:
	std::vector<SpawnTimer> m_SpawnQueue;
	float m_MinTimerDuration;
	float m_MaxTimerDuration;
	
	bool m_WasSlickSpawned;
	bool m_WasSamSpawned;
};

