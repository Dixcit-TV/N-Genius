#pragma once
#include <SDL_rect.h>

#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;

namespace ngenius
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, SDL_Rect* pSrcRect, SDL_Rect* pDestRect) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	
	private:
		friend class Singleton<Renderer>;
		explicit Renderer() = default;
		
		SDL_Renderer* m_Renderer{};
	};
}

