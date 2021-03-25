#pragma once
#include "IResource.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

struct SDL_Texture;
namespace ngenius
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D : public IResource
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		virtual ~Texture2D();

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

		SDL_Texture* GetSDLTexture() const;
		const glm::ivec2& GetTextureSize() const;
	
	protected:
		glm::ivec2 m_TextureSize;
		SDL_Texture* m_Texture;
	};
}
