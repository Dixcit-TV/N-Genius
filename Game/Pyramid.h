#pragma once
#include <IComponent.h>
#include <vector>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm.hpp>
#pragma warning(pop)

#include <algorithm>
#include "Delegate.h"
#include "Enums.h"

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

	void UpdateCell(const glm::vec2& playerPosition, bool forceRevertColor);
	void RegisterCompletionEvent(const std::string& evtName, const std::function<void()>& callback) { m_CompletionEvent.Register(evtName, callback); }
	void UnregisterCompletionEvent(const std::string& evtName) { m_CompletionEvent.Unregister(evtName); }
	void RegisterColorChangeEvent(const std::string& evtName, const std::function<void(ScoreEventType)>& callback) { m_ColorChangeEvent.Register(evtName, callback); }
	void UnregisterColorChangeEvent(const std::string& evtName) { m_ColorChangeEvent.Unregister(evtName); }
	
	size_t GetCellIdxFromWorldPos(const glm::vec2& position) const;
	glm::vec2 GetTargetPosition(const glm::vec2& position, const glm::vec2& direction) const;

	glm::vec2 GetTopPosition() const { return GetTopFacePosition(static_cast<int>(m_RowCount - 1), 0); }

private:
	std::vector<CellState> m_Blocks;
	std::vector<bool> m_Disks;
	ngenius::Event<ScoreEventType> m_ColorChangeEvent;
	ngenius::Event<> m_CompletionEvent;
	size_t m_RowCount;
	float m_CellSize;
	bool m_IsCellReverting;
	bool m_HasIntermediateColor;

	glm::vec2 GetPosition(int row, int column) const
	{
		const glm::vec2& gridPosition{ GetTransform().GetPosition() };
		return gridPosition + glm::vec2(m_CellSize * (2.f * column + row)
			, -m_CellSize * (3.f / 2.f * row));
	}

	glm::vec2 GetTopFacePosition(int row, int column) const
	{
		const glm::vec2 position{ GetPosition(row, column) };
		return position + glm::vec2(m_CellSize, -m_CellSize * 0.f);
	}

	void GetRowAndColumnFormPosition(const glm::vec2& position, int& r, int& c) const
	{
		const glm::vec2& gridPosition{ GetTransform().GetPosition() };
		glm::vec2 diff{ position - gridPosition };
		diff.y *= -1;
		diff.y += 1;
		
		c = static_cast<int>((0.5f * diff.x - 1.f / 3.f * diff.y) / m_CellSize);
		r = static_cast<int>(2.f / 3.f * diff.y / m_CellSize);
	}

	bool CheckCompletion() const
	{
		return !std::any_of(std::begin(m_Blocks), std::end(m_Blocks), [](CellState cellState) { return cellState != CellState::FINAL; });
	}
};

