#pragma once
#include <IComponent.h>

namespace ngenius {
	class Texture2D;
}

class LifeDisplay final : public ngenius::IComponent
{
public:
	explicit LifeDisplay(int initialLifeCount);
	~LifeDisplay() override = default;

	LifeDisplay(const LifeDisplay& rhs) = delete;
	LifeDisplay(LifeDisplay&& rhs) noexcept = delete;
	LifeDisplay& operator=(const LifeDisplay& rhs) = delete;
	LifeDisplay& operator=(LifeDisplay&& rhs) noexcept = delete;

	void Render() const override;
	void UpdateLifeDrawCount(int prevLifeCount, int newLifeCount);

private:
	std::shared_ptr<ngenius::Texture2D> m_pLifeTexture;
	int m_LifeCount;
};

