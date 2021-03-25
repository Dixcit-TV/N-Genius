#pragma once
#include <memory>
#include "IComponent.h"
#include "IObserver.h"

class HealthComponent final : public ngenius::IComponent, public ngenius::IObserver
{
public:
	explicit HealthComponent(int maxHealth);
	~HealthComponent() override = default;
	HealthComponent(const HealthComponent& rhs) = delete;
	HealthComponent(HealthComponent&& rhs) noexcept = delete;
	HealthComponent& operator=(const HealthComponent& rhs) = delete;
	HealthComponent& operator=(HealthComponent&& rhs) noexcept = delete;
	
	void OnNotify(MessageType event, void* pPayload) override;

	int GetHealth() const { return m_HealthCount; }

private:
	int m_HealthCount;

	void OnPlayerDied();
};

