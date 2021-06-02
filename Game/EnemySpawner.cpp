#include "EnemySpawner.h"
#include "FactoryMethod.h"
#include "HelperFunctions.h"
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
	QueueSpawn(EnemyType::SAM);
	QueueSpawn(EnemyType::UGG);
	QueueSpawn(EnemyType::WRONGWAY);
	QueueSpawn(EnemyType::EGG);
	QueueSpawn(EnemyType::EGG);
}

void EnemySpawner::QueueSpawn(EnemyType eType, bool instantSpawn)
{
	if (eType == EnemyType::SAM)
	{
		if (m_WasSamSpawned)
			return;

		m_WasSamSpawned = true;
	}

	if (eType == EnemyType::SLICK)
	{
		if (m_WasSlickSpawned)
			return;

		m_WasSlickSpawned = true;
	}
	
	m_SpawnQueue.push_back(SpawnTimer{ instantSpawn ? 0.f : Helpers::RandValue(m_MinTimerDuration, m_MaxTimerDuration), 0.f, eType });
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
				pGo = FactoryMethod::CreateSlickSam(ngenius::Transform(pyramidComp->GetTopPosition(CellFace::TOP)), pyramidComp, shared_from_this(), timer.spawnType);
				break;
			case EnemyType::UGG:
				pGo = FactoryMethod::CreateUggWrongWay(ngenius::Transform(pyramidComp->GetBottomRightPosition(CellFace::RIGHT)), shared_from_this(), timer.spawnType);
				break;
			case EnemyType::WRONGWAY:
				pGo = FactoryMethod::CreateUggWrongWay(ngenius::Transform(pyramidComp->GetBottomLeftPosition(CellFace::LEFT)), shared_from_this(), timer.spawnType);
				break;
			case EnemyType::EGG:
				pGo = FactoryMethod::CreateEgg(ngenius::Transform(pyramidComp->GetTopPosition(CellFace::TOP)), shared_from_this(), timer.spawnType);
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
