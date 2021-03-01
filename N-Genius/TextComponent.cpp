#include "PCH.h"
#include "TextComponent.h"

#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

ngenius::TextComponent::TextComponent(const std::string& text, const std::function<std::string()>& pTextBindFnc, const std::shared_ptr<Font>& pfont, const SDL_Color& color, const TransformComponent& transform)
	: IComponent()
	, m_LocalTransform(transform)
	, m_Color(color)
	, m_pFont(pfont)
	, m_pTexture(nullptr)
	, m_pTextBindFnc(pTextBindFnc)
	, m_Text(pTextBindFnc ? pTextBindFnc() : text)
	, m_NeedsUpdate(false)
{
	SetTextTexture();
}

void ngenius::TextComponent::SetTextTexture()
{
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
	std::cout << std::boolalpha << "Should Update: " << m_NeedsUpdate << std::endl;
	if (m_pTextBindFnc)
	{
		SetText(m_pTextBindFnc());
	}

	if (m_NeedsUpdate)
	{
		SetTextTexture();
		m_NeedsUpdate = false;
	}
}

void ngenius::TextComponent::Render() const
{
	const auto pos = m_LocalTransform.GetPosition();
	const auto parentPos = m_pGameObject.lock()->GetTransform()->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, parentPos.x + pos.x, parentPos.y + pos.y);
}

// This implementation uses the "dirty flag" pattern
void ngenius::TextComponent::SetText(const std::string& text)
{
	if (text != m_Text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}

void ngenius::TextComponent::SetPosition(const float x, const float y)
{
	m_LocalTransform.SetPosition(x, y);
}

void ngenius::TextComponent::SetColor(const SDL_Color& newColor)
{
	m_Color.r = newColor.r;
	m_Color.g = newColor.g;
	m_Color.b = newColor.b;
	m_Color.a = newColor.a;

	m_NeedsUpdate = true;
}
