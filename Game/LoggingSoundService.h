#pragma once
#include <string>
#include "ISoundService.h"

class LoggingSoundService : public ISoundService
{
public:
	explicit LoggingSoundService() = default;
	~LoggingSoundService() override = default;

	void PlaySound(const std::string& soundFile, int volume) override;
	void Update() override;

private:
	ISoundService* m_pSoundService;
};

