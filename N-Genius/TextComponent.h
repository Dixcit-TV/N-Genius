#pragma once
#include <SDL_pixels.h>

#include "IComponent.h"
#include "Delegate.h"
#include "Transform.h"

namespace ngenius
{
	class Texture2D;
	class Font;
	
	class TextComponent final : public IComponent
	{
	public:
		explicit TextComponent(const std::string& text, const std::shared_ptr<Font>& pfont, const SDL_Color& color = { 255, 255, 255 });
		~TextComponent() override = default;
		TextComponent(const TextComponent& rhs) = delete;
		TextComponent(TextComponent&& rhs) noexcept = delete;
		TextComponent& operator=(const TextComponent& rhs) = delete;
		TextComponent& operator=(TextComponent&& rhs) noexcept = delete;
		
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& newColor);

	private:
		SDL_Color m_Color;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTexture; 
		std::string m_Text;
		bool m_NeedsUpdate;

		void SetTextTexture();
	};
}
