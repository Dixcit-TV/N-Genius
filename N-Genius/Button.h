#pragma once
#include "TextComponent.h"

namespace ngenius
{
	class Button final : public TextComponent
	{
	public:
		explicit Button(const std::string& text, const std::shared_ptr<Font>& pfont, const SDL_Color& color = { 255, 255, 255 }, const glm::vec2& pivot = { 0.f, 0.f });
		~Button() override = default;
		Button(const Button& rhs) = default;
		Button(Button&& rhs) noexcept = default;
		Button& operator=(const Button& rhs) = default;
		Button& operator=(Button&& rhs) noexcept = default;
		
		void RegisterOnSelectedEvent(const std::string& evtName, std::function<void()> callback) { OnSelected.Register(evtName, std::move(callback)); }
		void RegisterOnClickEvent(const std::string& evtName, std::function<void()> callback) { OnClick.Register(evtName, std::move(callback)); }

		void Update() override;

	private:
		Event<> OnSelected;
		Event<> OnClick;

		bool m_IsSelected;
	};
}
