#pragma once
#include <string>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>
#pragma warning(pop)

#include "IComponent.h"

namespace ngenius
{
	class Texture2D;

	class SpriteComponent final : public IComponent
	{
	public:
		explicit SpriteComponent(const std::string& file, int sheetColumnCount, int sheetFrameCount, int numCols
			, int frameCount, int startColumn, int startRow, float frameTime, bool shouldRepeat);

		explicit SpriteComponent(const std::string& file, const glm::ivec2& frameSize, int numCols
			, int frameCount, int startColumn, int startRow, float frameTime, bool shouldRepeat);
		
		~SpriteComponent() override = default;
		SpriteComponent(const SpriteComponent& rhs) = delete;
		SpriteComponent(SpriteComponent&& rhs) noexcept = delete;
		SpriteComponent& operator=(const SpriteComponent& rhs) = delete;
		SpriteComponent& operator=(SpriteComponent&& rhs) noexcept = delete;

		bool IsDone() const;

		void NextFrame(float elapsedSec);
		void ResetAnimation();

		void Render() const override;

	private:
		glm::ivec2 m_FrameSize;
		
		std::shared_ptr<Texture2D> m_pSpriteSheet;
		int m_CurrFrame;
		int m_NumColumns;
		int m_FrameCount;
		int m_StartColumn;
		int m_StartRow;
		float m_FrameTime;
		float m_ElapsedTime;
		bool m_ShouldRepeat;
		bool m_IsDone;
	};
}

