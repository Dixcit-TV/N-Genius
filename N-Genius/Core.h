#pragma once
#include <string>

struct SDL_Window;
namespace ngenius
{
	class Core
	{
	public:
		void Initialize(const std::string& windowName, int width, int height);
		void Run();
	private:
		SDL_Window* m_Window{};

		void Cleanup();
	};
}