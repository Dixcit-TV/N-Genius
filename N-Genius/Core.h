#pragma once
struct SDL_Window;
namespace ngenius
{
	class Core
	{
	public:
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();
	private:
		SDL_Window* m_Window{};
	};
}