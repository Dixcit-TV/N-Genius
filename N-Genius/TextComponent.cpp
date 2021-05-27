#include "PCH.h"
#include "TextComponent.h"

#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

ngenius::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font>& pfont, const SDL_Color& color)
	: IComponent()
	, m_Color(color)
	, m_pFont(pfont)
	, m_pTexture(nullptr)
	, m_Text("")
	, m_NeedsUpdate()
{
	SetText(text);
}

void ngenius::TextComponent::SetTextTexture()
{
	if (m_Text.size() == 0 || m_Text[0] == '\0')
	{
		m_pTexture = nullptr;
		return;
	}
	
	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
	
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_pTexture = std::make_shared<Texture2D>(texture);
}

void ngenius::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		SetTextTexture();
		m_NeedsUpdate = false;
	}
}

void ngenius::TextComponent::Render() const
{
	if (m_pTexture)
	{
		const auto parentPos = GetTransform().GetPosition();
		
		Renderer::GetInstance().RenderTexture(m_pTexture->GetSDLTexture(), parentPos, m_pTexture->GetTextureSize());
	}
}

// This implementation uses the "dirty flag" pattern
void ngenius::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void ngenius::TextComponent::SetColor(const SDL_Color& newColor)
{
	m_Color.r = newColor.r;
	m_Color.g = newColor.g;
	m_Color.b = newColor.b;
	m_Color.a = newColor.a;

	m_NeedsUpdate = true;
}
