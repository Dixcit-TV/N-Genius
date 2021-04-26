#pragma once
#include "ISoundService.h"
#include "NullSoundService.h"

class ServiceLocator
{
	static NullSoundService m_NullSoundService;
	static ISoundService* m_pSDLSoundService;
public:
	static void RegisterSoundService(ISoundService* pSoundService)
	{
		if (m_pSDLSoundService && m_pSDLSoundService != &m_NullSoundService)
			delete m_pSDLSoundService;

		m_pSDLSoundService = pSoundService ? pSoundService : &m_NullSoundService;
	}
	
	static ISoundService* GetSoundService()
	{
		return m_pSDLSoundService;
	}

	static void UnRegister()
	{
		RegisterSoundService(nullptr);
	}
};

NullSoundService ServiceLocator::m_NullSoundService;
ISoundService* ServiceLocator::m_pSDLSoundService = &m_NullSoundService;

