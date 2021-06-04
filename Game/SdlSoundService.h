#pragma once
#include "ISoundService.h"
#include <queue>
#include <unordered_map>
#include "SoundEffect.h"
#include <mutex>

class SdlSoundService final : public ISoundService
{
public:
	explicit SdlSoundService();
	~SdlSoundService() override;

	void PlaySound(const std::string& soundFile, int volume) override;

private:
	struct SoundRequest
	{
		std::string soundName;
		int volume;
	};
	
	std::queue<SoundRequest> m_SoundQueue;
	std::unordered_map<std::string, SoundEffect> m_Sounds;
	std::mutex m_SoundMutex;
	std::condition_variable m_WorkerVariable;
	std::atomic_bool m_IsThreadRunning;
	std::thread m_SoundThread;

	void Play(const SoundRequest& soundRequest);
	void Update() override;
};

