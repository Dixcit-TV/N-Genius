#include "Pyramid.h"

#include <iostream>
#include <Texture2D.h>
#include <Renderer.h>
#include <ResourceManager.h>

Pyramid::Pyramid(size_t rowCount, float cellSize, bool cellReverting, bool intermediateColor)
	: m_CellMatrix(glm::mat2x2(2.f, 0.f, 1.f, 3.f / 2.f))
	, m_Blocks()
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
			const glm::vec2 position{ GetPosition(r, c, CellFace::TOP) - glm::vec2(m_CellSize, m_CellSize) };
			
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

void Pyramid::UpdateCell(const glm::vec2& playerPosition, bool forceRevertColor)
{
	if (!IsInBound(playerPosition))
	{
		std::cout << "Cell Out of Range" << std::endl;
		return;
	}

	const size_t cellIdx{ GetCellIdxFromWorldPos(playerPosition) };
	std::cout << "Updated Cell: " << cellIdx << std::endl;
	const CellState prevCellState{ m_Blocks[cellIdx] };
	CellState newState{ prevCellState };
	bool hasReverted{ false };
	
	switch (prevCellState)
	{
	case CellState::INITIAL:
		newState = forceRevertColor ? newState : (m_HasIntermediateColor ? CellState::INTERMEDIATE : CellState::FINAL);
		break;
	case CellState::INTERMEDIATE:
		hasReverted = forceRevertColor;
		newState = hasReverted ? CellState::INITIAL : CellState::FINAL;
		break;
	case CellState::FINAL:
		hasReverted = m_IsCellReverting || forceRevertColor;
		newState = hasReverted ? (m_HasIntermediateColor ? CellState::INTERMEDIATE : CellState::INITIAL) : newState;
		break;
	}

	m_Blocks[cellIdx] = newState;
	
	if (newState != prevCellState && !hasReverted)
		m_ColorChangeEvent.Invoke(ScoreEventType::COLOR_CHANGE);
	
	if (newState != prevCellState && newState == CellState::FINAL 
		&& CheckCompletion())
		m_CompletionEvent.Invoke();
}

size_t Pyramid::GetCellIdxFromWorldPos(const glm::vec2& position) const
{
	int r, c;
	GetRowAndColumnFromPosition(position, r, c);
	return GetIndex(r, c);
}

int Pyramid::GetIndex(int row, int col) const
{
	int rTotal{ 0 };
	for (int r2{ 0 }; r2 < row; ++r2)
		rTotal += r2;

	return col + row * static_cast<int>(m_RowCount) - rTotal;
}

void Pyramid::GetTargetPosition(const glm::vec2& position, const glm::vec2& direction, CellFace face, glm::vec2& targetPos, bool& isTargetOut) const
{
	int r, c;
	GetRowAndColumnFromPosition(position, r, c);
	c += static_cast<int>(direction.x);
	r += static_cast<int>(direction.y);

	targetPos = GetFacePosition(r, c, face);
	isTargetOut = !IsInBound(r, c);
}

glm::vec2 Pyramid::GetPosition(int row, int column, CellFace) const
{
	const glm::vec2& gridPosition{ GetTransform().GetPosition() };

	return gridPosition + m_CellMatrix * glm::vec2(column, row) * glm::vec2(m_CellSize, -m_CellSize);
}

glm::vec2 Pyramid::GetFacePosition(int row, int column, CellFace face) const
{
	const glm::vec2 position{ GetPosition(row, column, face) };
	glm::vec2 offset{ 0.f, 0.f };

	switch (face)
	{
	case CellFace::TOP:
		offset.y = -m_CellSize * 0.5f;
		break;
	case CellFace::LEFT:
		offset.x = -m_CellSize * 0.5f;
		offset.y = m_CellSize * 0.5f;
		break;
	case CellFace::RIGHT:
		offset.x = m_CellSize * 0.5f;
		offset.y = m_CellSize * 0.5f;
		break;
	default: break;
	}

	return position + offset;
}

//https://www.redblobgames.com/grids/hexagons/#comment-1063818420 - pixel to Hex Coordinates conversion
void Pyramid::GetRowAndColumnFromPosition(const glm::vec2& position, int& r, int& c) const
{
	const glm::vec2& gridPosition{ GetTransform().GetPosition() };
	glm::vec2 diff{ position - glm::fvec2(m_CellSize, m_CellSize) - gridPosition };
	diff.y *= -1;
	diff.x = roundf(diff.x);
	diff.y = roundf(diff.y);

	float x = (diff.x - m_CellSize) / (m_CellSize * 2.f);

	const float t1 = diff.y / m_CellSize, t2 = floorf(x + t1);
	r = static_cast<int>(floorf((floorf(t1 - x) + t2) / 3.f));
	c = static_cast<int>(floorf((floorf(2.f * x + 1.f) + t2) / 3.f)) - r + 1;
}

bool Pyramid::IsInBound(const glm::vec2& position) const
{
	int r, c;
	GetRowAndColumnFromPosition(position, r, c);
	return IsInBound(r, c);
}

bool Pyramid::IsInBound(int row, int col) const
{
	return row >= 0 && row < static_cast<int>(m_RowCount) && col >= 0 && col <= static_cast<int>(m_RowCount) - (row + 1);
}

void Pyramid::Update()
{
	//glm::ivec2 mousePos{ ngenius::InputManager::GetMousePosition() };
	//int r, c;
	//GetRowAndColumnFromPosition(mousePos, r, c);
	//std::cout << "MousePos =====> x: " << mousePos.x << ", y: " << mousePos.y << std::endl;
	//std::cout << "Targeted Cell =====> r: " << r << ", c: " << c << std::endl;
}