#include "EnemySpawner.h"
#include "FactoryMethod.h"
#include "TimeSettings.h"
#include "Pyramid.h"

EnemySpawner::EnemySpawner()
	: IComponent()
	, m_SpawnQueue()
	, m_MinTimerDuration(5.f)
	, m_MaxTimerDuration(10.f)
	, m_WasSlickSpawned(false)
	, m_WasSamSpawned(false)
{
	float r{ static_cast<float>(rand()) / RAND_MAX * (m_MaxTimerDuration - m_MinTimerDuration) + m_MinTimerDuration };
	m_SpawnQueue.push_back(SpawnTimer{ r, 0.f, EnemyType::SAM });

	r = static_cast<float>(rand()) / RAND_MAX * (m_MaxTimerDuration - m_MinTimerDuration) + m_MinTimerDuration;
	m_SpawnQueue.push_back(SpawnTimer{ r, 0.f, EnemyType::UGG });

	r = static_cast<float>(rand()) / RAND_MAX * (m_MaxTimerDuration - m_MinTimerDuration) + m_MinTimerDuration;
	m_SpawnQueue.push_back(SpawnTimer{ r, 0.f, EnemyType::WRONGWAY });

	r = static_cast<float>(rand()) / RAND_MAX * (m_MaxTimerDuration - m_MinTimerDuration) + m_MinTimerDuration;
	m_SpawnQueue.push_back(SpawnTimer{ r, 0.f, EnemyType::EGG });

	r = static_cast<float>(rand()) / RAND_MAX * (m_MaxTimerDuration - m_MinTimerDuration) + m_MinTimerDuration;
	m_SpawnQueue.push_back(SpawnTimer{ r, 0.f, EnemyType::EGG });
}

void EnemySpawner::Update()
{
	bool spawnCompleted{ false };
	const float deltaTime{ngenius::TimeSettings::GetInstance().GetElapsed() };
	for (SpawnTimer& timer : m_SpawnQueue)
	{
		timer.timer += deltaTime;
		if (timer.timer >= timer.duration)
		{
			spawnCompleted = true;
			std::shared_ptr<Pyramid> pyramidComp{ GetComponent<Pyramid>() };
			if (!pyramidComp)
				return;

			auto pScene{ m_pGameObject.lock()->GetScene() };
			std::shared_ptr<ngenius::GameObject> pGo{ nullptr };
			switch (timer.spawnType)
			{
			case EnemyType::SAM:
			case EnemyType::SLICK:
				pGo = FactoryMethod::CreateSlickSam(ngenius::Transform(pyramidComp->GetTopPosition(CellFace::TOP)), pyramidComp, timer.spawnType);
				break;
			case EnemyType::UGG:
				pGo = FactoryMethod::CreateUggWrongWay(ngenius::Transform(pyramidComp->GetBottomRightPosition(CellFace::RIGHT)), timer.spawnType);
				break;
			case EnemyType::WRONGWAY:
				pGo = FactoryMethod::CreateUggWrongWay(ngenius::Transform(pyramidComp->GetBottomLeftPosition(CellFace::LEFT)), timer.spawnType);
				break;
			case EnemyType::EGG:
				pGo = FactoryMethod::CreateEgg(ngenius::Transform(pyramidComp->GetBottomLeftPosition(CellFace::LEFT)), timer.spawnType);
				break;
			case EnemyType::COILY: break;
			}

			if (pGo)
				pScene->Add(pGo);
		}
	}

	if (spawnCompleted)
		m_SpawnQueue.erase(std::remove_if(std::begin(m_SpawnQueue), std::end(m_SpawnQueue), [](const SpawnTimer& spTimer) { return spTimer.timer >= spTimer.duration; }), std::end(m_SpawnQueue));
}
