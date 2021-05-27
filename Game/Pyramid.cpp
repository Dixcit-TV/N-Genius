#include "Pyramid.h"

#include <iostream>

#include "Renderer.h"
#include "ResourceManager.h"

Pyramid::Pyramid(size_t rowCount, float cellSize, bool cellReverting, bool intermediateColor)
	: m_Blocks()
	, m_CompletionEvent()
	, m_RowCount(rowCount)
	, m_CellSize(cellSize)
	, m_IsCellReverting(cellReverting)
	, m_HasIntermediateColor(intermediateColor)
{
	const float rowCountF{ static_cast<float>(rowCount) };
	const float cellCount{ rowCountF * (rowCountF * 0.5f + 0.5f) };
	m_Blocks.resize(static_cast<size_t>(cellCount), CellState::INITIAL);
}

void Pyramid::Render() const
{
	auto t{ngenius::ResourceManager::GetInstance().LoadResource<ngenius::Texture2D>("Sprites/QBert.png") };

	int columnCount{ static_cast<int>(m_RowCount) };
	int rSum{ 0 };
	for (int r{}; r < static_cast<int>(m_RowCount); ++r)
	{
		for (int c{ }; c < columnCount; ++c)
		{
			const glm::vec2 position{ GetPosition(r, c) };
			
			SDL_Rect dest{};
			dest.x = static_cast<int>(position.x);
			dest.y = static_cast<int>(position.y);
			dest.w = static_cast<int>(2 * m_CellSize);
			dest.h = static_cast<int>(2 * m_CellSize);
			
			SDL_Rect src{};
			src.w = 32;
			src.h = 32;
			src.x = 0;
			src.y = 160 + static_cast<int>(m_Blocks[c + m_RowCount * r - rSum]) * src.h;
			
			ngenius::Renderer::GetInstance().RenderTexture(t->GetSDLTexture(), src, dest);
		}
		rSum += r;
		--columnCount;
	}
}

void Pyramid::UpdateCell(const glm::vec2& playerPosition)
{
	const size_t celIdx{ GetCellIdxFromWorldPos(playerPosition) };
	std::cout << "Updated Cell: " << celIdx << std::endl;
	const CellState prevCellState{ m_Blocks[celIdx] };
	CellState newState{ prevCellState };
	
	switch (prevCellState)
	{
	case CellState::INITIAL:
		newState = m_HasIntermediateColor ? CellState::INTERMEDIATE : CellState::FINAL;
		break;
	case CellState::INTERMEDIATE:
		newState = CellState::FINAL;
		break;
	case CellState::FINAL:
		newState = m_IsCellReverting ? (m_HasIntermediateColor ? CellState::INTERMEDIATE : CellState::INITIAL) : newState;
		break;
	}

	m_Blocks[celIdx] = newState;
	const bool needCheck{ newState != prevCellState && newState == CellState::FINAL };
	
	if (needCheck && CheckCompletion())
		m_CompletionEvent.Invoke();
}

size_t Pyramid::GetCellIdxFromWorldPos(const glm::vec2& position) const
{
	size_t r, c;
	GetRowAndColumnFormPosition(position, r, c);

	size_t rTotal{ 0 };
	for (size_t r2{ 0 }; r2 < r; ++r2)
		rTotal += r2;
	
	return c + r * m_RowCount - rTotal;
}

glm::vec2 Pyramid::GetTargetPosition(const glm::vec2& position, const glm::vec2& direction) const
{
	size_t r, c;
	GetRowAndColumnFormPosition(position, r, c); 
	c += static_cast<size_t>(direction.x);
	r += static_cast<size_t>(direction.y);
	
	std::cout << "Targeted Cell =====> r: " << r << ", c: " << c << std::endl;
	
	return GetTopFacePosition(r, c); //Add 0.5f * m_CellSize to offset the target position to the "center" of the top face of the cube
}