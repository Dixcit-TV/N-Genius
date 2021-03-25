#include "SpriteComponent.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"

ngenius::SpriteComponent::SpriteComponent(const std::string& file, int sheetColumnCount, int sheetFrameCount, int numCols
    , int frameCount, int startColumn, int startRow, float frameTime, bool shouldRepeat)
	: IComponent()
	, m_pSpriteSheet(ResourceManager::GetInstance().LoadResource<SpriteSheet>(file, sheetColumnCount, sheetFrameCount))
	, m_CurrFrame(0)
	, m_NumColumns(numCols)
	, m_FrameCount(frameCount)
	, m_StartColumn(startColumn)
	, m_StartRow(startRow)
	, m_FrameTime(frameTime)
	, m_ElapsedTime(0.f)
	, m_ShouldRepeat(shouldRepeat)
	, m_IsDone(false)
{
}

bool ngenius::SpriteComponent::IsDone() const
{
	return m_IsDone;
}

void ngenius::SpriteComponent::NextFrame(float elapsedSec, bool repeat)
{
	if (!repeat && m_CurrFrame == m_FrameCount - 1)
	{
		m_IsDone = true;
		return;
	}

	m_ElapsedTime += elapsedSec;
	if (m_ElapsedTime >= m_FrameTime)
	{
		++m_CurrFrame %= m_FrameCount;
		m_ElapsedTime -= m_FrameTime;
	}
}

void ngenius::SpriteComponent::ResetAnimation()
{
	m_CurrFrame = 0;
	m_ElapsedTime = 0.f;
	m_IsDone = false;
}

void ngenius::SpriteComponent::Render() const
{
	const auto& frameSize{ m_pSpriteSheet->GetFrameSize() };
	SDL_Rect src{ };
	src.w = frameSize.x;
	src.h = frameSize.y;
	src.x = src.w * (m_StartColumn + m_CurrFrame % m_NumColumns);
	src.y = src.h * (m_StartRow + m_CurrFrame / m_NumColumns);

	const Transform& parentTransform = m_pGameObject->GetTransform();
	const auto& scale = parentTransform.GetScale();
	const auto& pos = parentTransform.GetPosition();
	SDL_Rect dest{ };
	dest.w = static_cast<int>(scale.x * src.w);
	dest.h = static_cast<int>(scale.y * src.h);
	dest.x = static_cast<int>(pos.x);
	dest.y = static_cast<int>(pos.y);
	
	Renderer::GetInstance().RenderTexture(m_pSpriteSheet->GetSDLTexture(), src, dest);
}