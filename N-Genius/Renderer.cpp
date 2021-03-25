#include "PCH.h"
#include "Renderer.h"
#include <SDL.h>

#include "imgui.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

void ngenius::Renderer::Init(SDL_Window * window)
{
	m_pWindow = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void ngenius::Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();

	bool showDemo{true};
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pWindow);
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(&showDemo);
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_Renderer);
}

void ngenius::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void ngenius::Renderer::RenderTexture(SDL_Texture* pTexture, const glm::vec2& position, const glm::ivec2& size, const glm::vec2& scale) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(position.x);
	dst.y = static_cast<int>(position.y);
	dst.w = static_cast<int>(size.x * scale.x);
	dst.h = static_cast<int>(size.y * scale.y);
	SDL_RenderCopy(GetSDLRenderer(), pTexture, nullptr, &dst);
}

void ngenius::Renderer::RenderTexture(SDL_Texture* pTexture, const SDL_Rect& srcRect, const SDL_Rect& destRect) const
{
	SDL_RenderCopy(GetSDLRenderer(), pTexture, &srcRect, &destRect);
}

int ngenius::Renderer::GetOpenGLDriverIndex() const
{
	int openGLIndex{ -1 };
	const int driverCount{ SDL_GetNumRenderDrivers() };
	for (int i{}; i < driverCount; ++i)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
		{
			if (!strcmp(info.name, "opengl"))
			{
				openGLIndex = i;
				break;
			}
		}
	}

	return openGLIndex;
}
