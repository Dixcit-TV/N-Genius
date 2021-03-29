#pragma once
#include <string>
#include <SDL_mixer.h>

class SoundEffect
{
public:
	explicit SoundEffect( const std::string& path );
	~SoundEffect( );
	SoundEffect(const SoundEffect& other) = delete;
	SoundEffect& operator=(const SoundEffect& other) = delete;
	SoundEffect( SoundEffect&& other) noexcept
		: m_pMixChunk{ other.m_pMixChunk }
	{
		other.m_pMixChunk = nullptr;
	}
	
	SoundEffect& operator=(SoundEffect&& other) noexcept
	{
		m_pMixChunk = other.m_pMixChunk;
		other.m_pMixChunk = nullptr;
		return *this;
	}

	bool IsLoaded( ) const;
	bool Play( int loops ) const;
	void SetVolume( int value ); 
	int GetVolume( ) const; 
	static void StopAll( );
	static void PauseAll( );
	static void ResumeAll( );

private:
	Mix_Chunk* m_pMixChunk;
};
