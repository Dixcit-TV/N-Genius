#include "ServiceLocator.h"

NullSoundService ServiceLocator::m_NullSoundService;
ISoundService* ServiceLocator::m_pSDLSoundService = &m_NullSoundService;