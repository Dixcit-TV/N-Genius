#pragma once
#include "ISoundService.h"

class NullSoundService final : public ISoundService
{
public:
	explicit NullSoundService() = default;
	~NullSoundService() override = default;
	
	void PlaySound(const std::string&, int) override {}
	void Update() override {}
};

