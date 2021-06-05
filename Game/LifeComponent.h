#pragma once
#include <IComponent.h>
#include "Delegate.h"

class LifeComponent : public ngenius::IComponent
{
public:
	explicit LifeComponent(int lifeCount);
	~LifeComponent() override = default;

	LifeComponent(const LifeComponent& rhs) = delete;
	LifeComponent(LifeComponent&& rhs) noexcept = delete;
	LifeComponent& operator=(const LifeComponent& rhs) = delete;
	LifeComponent& operator=(LifeComponent&& rhs) noexcept = delete;

	void RegisterUpdateEvent(const std::string& evtName, const std::function<void(int, int)>& callback) { m_OnHealthChangeEvent.Register(evtName, callback); }
	void RegisterHealthDepletedEvent(const std::string& evtName, const std::function<void()>& callback) { m_OnHealthDepletedEvent.Register(evtName, callback); }
	void ApplyDamage(int damageAmount);

	int GetHealth() const { return m_LifeCount; }
	void SetHealth(int newHealth)
	{
		ApplyDamage(-(newHealth - m_LifeCount));
	}

private:
	ngenius::Event<> m_OnHealthDepletedEvent;
	ngenius::Event<int, int> m_OnHealthChangeEvent;
	int m_LifeCount;
};

