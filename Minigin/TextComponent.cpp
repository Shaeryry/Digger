#include "TextComponent.h"
#include <stdexcept>
#include <algorithm>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(GameObject* gameObject, Font* font) :
	TextComponent(gameObject, "TextComponent", font)
{
}

dae::TextComponent::TextComponent(GameObject* gameObject, const std::string& text, Font* font) :
	TextureComponent(gameObject),
	m_Changed{ true }
{
	SetText(text);
	SetFont(font);
	SetColor(255, 255, 255);
}


void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_Changed = true;
}

void dae::TextComponent::SetFont(Font* font)
{
	m_Font = std::move(font);
	m_Changed = true;
}

void dae::TextComponent::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Color newColor{ r,g,b,a };
	SetColor(newColor);
}

void dae::TextComponent::SetColor(SDL_Color color)
{
	/*color.r = std::clamp(color.r, Uint8(0) , Uint8(255));
	color.g = std::clamp(color.g, Uint8(0) , Uint8(255));
	color.b = std::clamp(color.b, Uint8(0) , Uint8(255));
	color.a = std::clamp(color.a, Uint8(0) , Uint8(255));*/

	m_Color = color;
	m_Changed = true;
}

void dae::TextComponent::Update()
{ 
	if (m_Changed) {

		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
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

		m_TextTexture = std::make_unique<Texture2D>(texture);
		SetTexture( m_TextTexture.get() );
		m_Changed = false;
	}
}
