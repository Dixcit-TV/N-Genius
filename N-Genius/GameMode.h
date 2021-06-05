#pragma once

namespace ngenius
{
	class GameMode
	{
	public:
		explicit GameMode() = default;
		virtual ~GameMode() = default;
		GameMode(const GameMode& other) = default;
		GameMode(GameMode&& other) = default;
		GameMode& operator=(const GameMode& other) = default;
		GameMode& operator=(GameMode&& other) = default;

		virtual void Update() {}
	};
}

