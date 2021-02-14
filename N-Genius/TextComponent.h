#pragma once
#include "IComponent.h"
#include "Delegate.h"
#include "TransformComponent.h"

namespace ngenius
{
	class Texture2D;
	class Font;
	
	class TextComponent final : public IComponent
	{
	public:
		explicit TextComponent(const std::string& text, const std::function<std::string()>& pTextBindFnc, const std::shared_ptr<Font>& pfont, const TransformComponent& transform = TransformComponent{});

		void Receive(eComponentMessage message, const std::shared_ptr<GameObject>& pParentGo) override;
		
		void Update();
		void Render(const TransformComponent& parentTransform) const;

		void SetText(const std::string& text);
		void SetTextPosition(float x, float y);

	private:
		TransformComponent m_LocalTransform;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTexture;
		std::function<std::string()> m_pTextBindFnc;
		std::string m_Text;
		bool m_NeedsUpdate;

		void SetTextTexture();
	};
}
