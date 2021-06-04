#pragma once
#include <SDL_pixels.h>

#include "IComponent.h"
#include "Delegate.h"
#include "Transform.h"

namespace ngenius
{
	class Texture2D;
	class Font;
	
	class TextComponent : public IComponent
	{
	public:
		explicit TextComponent(const std::string& text, const std::shared_ptr<Font>& pfont, const SDL_Color& color = { 255, 255, 255 }, const glm::vec2& pivot = {0.f, 0.f});
		~TextComponent() override = default;
		TextComponent(const TextComponent& rhs) = default;
		TextComponent(TextComponent&& rhs) noexcept = default;
		TextComponent& operator=(const TextComponent& rhs) = default;
		TextComponent& operator=(TextComponent&& rhs) noexcept = default;
		
		virtual void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& newColor);

	protected:
		SDL_Color m_Color;
		glm::vec2 m_Pivot;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTexture; 
		std::string m_Text;
		bool m_NeedsUpdate;

		void SetTextTexture();
	};
}
