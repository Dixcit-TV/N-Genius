#pragma once
#include <IComponent.h>
#include <vector>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>
#pragma warning(pop)

#include <algorithm>

#include "Delegate.h"

enum class CellState
{
	INITIAL
	, INTERMEDIATE
	, FINAL
};

class Pyramid final: public ngenius::IComponent
{
public:
	explicit Pyramid(size_t rowCount, float cellSize, bool cellReverting = false, bool intermediateColor = false);
	~Pyramid() override = default;
	
	Pyramid(const Pyramid& rhs) = default;
	Pyramid(Pyramid&& rhs) noexcept = default;
	Pyramid& operator=(const Pyramid& rhs) = default;
	Pyramid& operator=(Pyramid&& rhs) noexcept = default;
	void Render() const override;

	void UpdateCell(const glm::vec2& playerPosition);
	void RegisterCompletionEvent(const std::string& evtName, const std::function<void()>& callback) { m_CompletionEvent.Register(evtName, callback); }

	size_t GetCellIdxFromWorldPos(const glm::vec2& position) const;
	glm::vec2 GetTargetPosition(const glm::vec2& position, const glm::vec2& direction) const;

	glm::vec2 GetTopPosition() const { return GetTopFacePosition(m_RowCount - 1, 0); }

private:
	std::vector<CellState> m_Blocks;
	ngenius::Event<> m_CompletionEvent;
	size_t m_RowCount;
	float m_CellSize;
	bool m_IsCellReverting;
	bool m_HasIntermediateColor;

	glm::vec2 GetPosition(size_t row, size_t column) const
	{
		const glm::vec2& gridPosition{ GetTransform().GetPosition() };
		return gridPosition + glm::vec2(m_CellSize * (2.f * column + row)
			, -m_CellSize * (3.f / 2.f * row));
	}

	glm::vec2 GetTopFacePosition(size_t row, size_t column) const
	{
		glm::vec2 position{ GetPosition(row, column) };
		return position + glm::vec2(m_CellSize, 0.f);
	}

	void GetRowAndColumnFormPosition(const glm::vec2& position, size_t& r, size_t& c) const
	{
		const glm::vec2& gridPosition{ GetTransform().GetPosition() };
		glm::vec2 diff{ position - gridPosition };
		diff.y *= -1;
		diff.y += 1;
		
		c = static_cast<size_t>((0.5f * diff.x - 1.f / 3.f * diff.y) / m_CellSize);
		r = static_cast<size_t>(2.f / 3.f * diff.y / m_CellSize);
	}

	bool CheckCompletion() const
	{
		return !std::any_of(std::begin(m_Blocks), std::end(m_Blocks), [](CellState cellState) { return cellState != CellState::FINAL; });
	}
};

