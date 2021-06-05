#include "Game.h"

#include <Core.h>
#include <ResourceManager.h>
#include "GameModeManager.h"
#include "QbertGameMode.h"
#include "SdlSoundService.h"
#include "ServiceLocator.h"

using namespace ngenius;

int main(int, char* [])
{
	srand(unsigned(time(nullptr)));

	ServiceLocator::RegisterSoundService(new SdlSoundService());
	Core core{ "Q-bert", 960, 720 };
	ResourceManager::GetInstance().Init("../Data/");
	GameModeManager::SetGameMode(new QbertGameMode());
    core.Run();

	ServiceLocator::UnRegister();
    return 0;
}
