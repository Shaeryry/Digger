#pragma once
#include <memory>
#include <SDL_ttf.h>
#include "Texture2D.h"
#include "Component.h"

namespace dae {
	class Font;
	class TextureRendererComponent;
	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(GameObject* gameObject, TextureRendererComponent* textureRenderer, Font* font);
		explicit TextComponent(GameObject* gameObject, TextureRendererComponent* textureRenderer, const std::string& text, Font* font);
		void SetText(const std::string& text);
		void SetFont(Font* font);

		void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
		void SetColor(const SDL_Color& color);

		virtual void Update() override;
	private:

		TextureRendererComponent* m_Renderer;
		SDL_Color m_Color;
		Font* m_Font;

		std::unique_ptr<Texture2D> m_TextTexture{ };
		std::string m_Text;

		bool m_Changed;
	};
}


