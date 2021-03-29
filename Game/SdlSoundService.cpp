#include "SdlSoundService.h"

SdlSoundService::SdlSoundService()
	: m_SoundQueue()
	, m_Sounds()
	, m_SoundMutex()
	, m_WorkerVariable()
	, m_IsThreadRunning(true)
{
	std::thread newThread([this]() { Update(); });
	newThread.detach();
}

SdlSoundService::~SdlSoundService()
{
	SoundEffect::StopAll();
	m_IsThreadRunning.store(false);
	m_WorkerVariable.notify_one();
}

void SdlSoundService::PlaySound(const std::string& soundFile, int volume)
{
	std::lock_guard<std::mutex> lock(m_SoundMutex);
	m_SoundQueue.push(SoundRequest{ soundFile, volume });
	m_WorkerVariable.notify_one();
}

void SdlSoundService::Update()
{
	while(m_IsThreadRunning)
	{
		std::unique_lock<std::mutex> lock(m_SoundMutex);
		if (!m_SoundQueue.empty())
		{
			Play(m_SoundQueue.front());
			m_SoundQueue.pop();
		}
		else
			m_WorkerVariable.wait(lock);
	}
}

void SdlSoundService::Play(const SoundRequest& soundRequest)
{
	auto cIt = m_Sounds.find(soundRequest.soundName);
	if (cIt != m_Sounds.cend())
	{
		cIt->second.SetVolume(soundRequest.volume);
		cIt->second.Play(false);
		return;
	}

	SoundEffect newSF{ soundRequest.soundName };
	newSF.SetVolume(soundRequest.volume);
	newSF.Play(false);
	m_Sounds.emplace(soundRequest.soundName, std::move(newSF));
}