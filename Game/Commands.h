#pragma once
#include "LoggingSoundService.h"
#include "SdlSoundService.h"
#include "ServiceLocator.h"
#include "../N-Genius/Command.h"

class PlayHitSoundCommand final : public ngenius::ICommand
{
public:
	explicit PlayHitSoundCommand(std::shared_ptr<ngenius::GameObject> pTargetObject) : ICommand(std::move(pTargetObject)) {}

	void Execute(const ngenius::InputData& inputData) override
	{
		if (inputData.state == ngenius::InputState::PRESSED)
		{
			ServiceLocator::GetSoundService()->PlaySound("../Data/Sounds/Hit.wav", 50);
		}
	}
};

class PlayMenuSoundCommand final : public ngenius::ICommand
{
public:
	explicit PlayMenuSoundCommand(std::shared_ptr<ngenius::GameObject> pTargetObject) : ICommand(std::move(pTargetObject)) {}

	void Execute(const ngenius::InputData& inputData) override
	{
		if (inputData.state == ngenius::InputState::PRESSED)
		{
			ServiceLocator::GetSoundService()->PlaySound("../Data/Sounds/NavigateMenu.wav", 50);
		}
	}
};

class SDLAudioCommand final : public ngenius::ICommand
{
public:
	explicit SDLAudioCommand(std::shared_ptr<ngenius::GameObject> pTargetObject) : ICommand(std::move(pTargetObject)) {}

	void Execute(const ngenius::InputData& inputData) override
	{
		if (inputData.state == ngenius::InputState::PRESSED)
		{
			ServiceLocator::RegisterSoundService(new SdlSoundService());
		}
	}
};

class LoggingAudioCommand final : public ngenius::ICommand
{
public:
	explicit LoggingAudioCommand(std::shared_ptr<ngenius::GameObject> pTargetObject) : ICommand(std::move(pTargetObject)) {}

	void Execute(const ngenius::InputData& inputData) override
	{
		if (inputData.state == ngenius::InputState::PRESSED)
		{
			ServiceLocator::RegisterSoundService(new LoggingSoundService());
		}
	}
};