#include "Button.h"

#include "InputManager.h"
#include "Texture2D.h"

ngenius::Button::Button(const std::string& text, const std::shared_ptr<Font>& pfont, const SDL_Color& color, const glm::vec2& pivot)
	: TextComponent(text, pfont, color, pivot)
	, m_IsSelected(false)
{}

void ngenius::Button::Update()
{
	TextComponent::Update();
	
	const glm::vec2 mousePos{ InputManager::GetMousePosition() };
	const glm::vec2& goPos{ GetTransform().GetPosition() };
	const glm::vec2& scale{ GetTransform().GetScale() };
	glm::vec2 textSize{ m_pTexture->GetTextureSize() };
	textSize.x *= scale.x;
	textSize.y *= scale.y;
	
	const glm::vec2 topLeft(goPos.x - textSize.x * m_Pivot.x, goPos.y - textSize.y * m_Pivot.y);

	if (mousePos.x >= topLeft.x && mousePos.x <= topLeft.x + textSize.x
		&& mousePos.y >= topLeft.y && mousePos.y <= topLeft.y + textSize.y)
	{
		if (!m_IsSelected)
			OnSelected.Invoke();

		m_IsSelected = true;

		if (InputManager::GetInstance().GetSDLEvent(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT))
			OnClick.Invoke();
	}
	else
		m_IsSelected = false;
}
