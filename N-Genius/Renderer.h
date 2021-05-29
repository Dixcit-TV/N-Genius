#pragma once
#include <SDL_rect.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>
#pragma warning(pop)
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

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

		void RenderTexture(SDL_Texture* pTexture, const glm::vec2& position, const glm::ivec2& size, const glm::vec2& scale = glm::vec2{ 1.f, 1.f }, float rotation = 0.f, const glm::vec2& pivot = {0.f, 0.f}) const;
		void RenderTexture(SDL_Texture* pTexture, const SDL_Rect& srcRect, const SDL_Rect& destRect, float rotation = 0.f, const glm::vec2& pivot = { 0.f, 0.f }) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	
	private:
		friend class Singleton<Renderer>;
		explicit Renderer() = default;
		
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_pWindow{};

		int GetOpenGLDriverIndex() const;
	};
}

