#include "TextComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(GameObject* gameObject, std::shared_ptr<Font> font) :
	TextComponent(gameObject, "TextComponent", font)
{
}

dae::TextComponent::TextComponent(GameObject* gameObject, const std::string& text, std::shared_ptr<Font> font) :
	TextureComponent(gameObject),
	m_needsUpdate{ true }
{
	SetText(text);
	SetFont(font);
}


void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_font = std::move(font);
	m_needsUpdate = true;
}

void dae::TextComponent::Update()
{
	if (m_needsUpdate) {

		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
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
		//m_texture = std::make_shared<Texture2D>(texture);
		SetTexture( std::make_shared<Texture2D>(texture) );
		m_needsUpdate = false;
	}
}
