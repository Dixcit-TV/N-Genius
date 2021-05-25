#pragma once
#include <string>

struct SDL_Window;
namespace ngenius
{
	class Core
	{
	public:
		explicit Core(const std::string& windowName, int width, int height) { Initialize(windowName, width, height); }
		~Core() = default;
		Core(const Core&) = delete;
		Core(Core&&) noexcept = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) noexcept = delete;
		
		void Run();
	private:
		SDL_Window* m_Window{};
		bool m_IsInit{ false };

		void Initialize(const std::string& windowName, int width, int height);
		void Cleanup();	
	};
}