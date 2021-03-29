#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>
#pragma warning(pop)
#include <cstdint>
#include "Texture2D.h"

namespace ngenius
{
	class SpriteSheet final : public Texture2D
	{
	public:
		explicit SpriteSheet(SDL_Texture* texture, uint32_t frameCount, uint32_t columnCount);
		~SpriteSheet() override = default;
		
		SpriteSheet(const SpriteSheet&) = delete;
		SpriteSheet(SpriteSheet&&) = delete;
		SpriteSheet& operator= (const SpriteSheet&) = delete;
		SpriteSheet& operator= (const SpriteSheet&&) = delete;

		const glm::ivec2& GetFrameSize() const { return m_FrameSize; }
		uint32_t GetColumnCount() const { return m_ColumnCount; }
		uint32_t GetFrameCount() const { return m_FrameCount; }

	private:
		glm::ivec2 m_FrameSize;
		uint32_t m_ColumnCount;
		uint32_t m_FrameCount;
		
	};
}

