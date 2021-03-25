#pragma once
#include <string>
#include "IComponent.h"

namespace ngenius
{
	class SpriteSheet;

	class SpriteComponent final : public IComponent
	{
	public:
		explicit SpriteComponent(const std::string& file, int sheetColumnCount, int sheetFrameCount, int numCols
			, int frameCount, int startColumn, int startRow, float frameTime, bool shouldRepeat);
		
		~SpriteComponent() override = default;
		SpriteComponent(const SpriteComponent& rhs) = delete;
		SpriteComponent(SpriteComponent&& rhs) noexcept = delete;
		SpriteComponent& operator=(const SpriteComponent& rhs) = delete;
		SpriteComponent& operator=(SpriteComponent&& rhs) noexcept = delete;

		bool IsDone() const;

		void NextFrame(float elapsedSec, bool repeat = true);
		void ResetAnimation();

		void Render() const override;

	private:
		std::shared_ptr<SpriteSheet> m_pSpriteSheet;
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

