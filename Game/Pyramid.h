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
#include "Texture2D.h"

class Pyramid final: public ngenius::IComponent
{
public:
	explicit Pyramid(size_t rowCount, float cellSize, int colorIdx, bool cellReverting = false, bool intermediateColor = false);
	~Pyramid() override = default;
	
	Pyramid(const Pyramid& rhs) = delete;
	Pyramid(Pyramid&& rhs) noexcept = delete;
	Pyramid& operator=(const Pyramid& rhs) = delete;
	Pyramid& operator=(Pyramid&& rhs) noexcept = delete;
	void Update() override;
	void Render() const override;

	void UpdateCell(const glm::vec2& playerPosition, bool forceRevertColor);
	void RegisterCompletionEvent(const std::string& evtName, const std::function<void()>& callback) { m_CompletionEvent.Register(evtName, callback); }
	void UnregisterCompletionEvent(const std::string& evtName) { m_CompletionEvent.Unregister(evtName); }
	void RegisterColorChangeEvent(const std::string& evtName, const std::function<void(ScoreEventType)>& callback) { m_ColorChangeEvent.Register(evtName, callback); }
	void UnregisterColorChangeEvent(const std::string& evtName) { m_ColorChangeEvent.Unregister(evtName); }
	
	size_t GetCellIdxFromWorldPos(const glm::vec2& position) const;
	void GetTargetPosition(const glm::vec2& position, const glm::vec2& direction, CellFace face, glm::vec2& targetPos, bool& isTargetOut) const;

	glm::vec2 GetFacePosition(int row, int column, CellFace face = CellFace::TOP) const;
	glm::vec2 GetTopPosition(CellFace face = CellFace::TOP) const { return GetFacePosition(static_cast<int>(m_RowCount - 1), 0, face); }
	glm::vec2 GetBottomLeftPosition(CellFace face = CellFace::TOP) const { return GetFacePosition(0, 0, face); }
	glm::vec2 GetBottomRightPosition(CellFace face = CellFace::TOP) const { return GetFacePosition(0, static_cast<int>(m_RowCount - 1), face); }
	void GetRowAndColumnFromPosition(const glm::vec2& position, int& r, int& c) const;

private:
	glm::mat2x2 m_CellMatrix;
	std::vector<CellState> m_Blocks;
	std::vector<bool> m_Disks;
	ngenius::Event<ScoreEventType> m_ColorChangeEvent;
	ngenius::Event<> m_CompletionEvent;
	std::shared_ptr<ngenius::Texture2D> m_pTexture;
	size_t m_RowCount;
	int m_ColorIdx;
	float m_CellSize;
	bool m_IsCellReverting;
	bool m_HasIntermediateColor;

	bool IsInBound(const glm::vec2& position) const;
	bool IsInBound(int row, int col) const;
	int GetIndex(int row, int col) const;
	glm::vec2 GetPosition(int row, int column, CellFace face = CellFace::TOP) const;

	bool CheckCompletion() const
	{
		return !std::any_of(std::begin(m_Blocks), std::end(m_Blocks), [](CellState cellState) { return cellState != CellState::FINAL; });
	}
};

