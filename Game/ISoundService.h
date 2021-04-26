#pragma once
#include <string>

class ISoundService
{
public:
	virtual ~ISoundService() = default;
	ISoundService(const ISoundService&) = delete;
	ISoundService(ISoundService&&) noexcept = delete;
	ISoundService& operator=(const ISoundService&) = delete;
	ISoundService& operator=(ISoundService&&) noexcept = delete;

	virtual void PlaySound(const std::string& soundFile, int volume) = 0;

protected:
	explicit ISoundService() = default;

	virtual void Update() = 0;
};

