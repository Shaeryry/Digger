#pragma once
#include <memory>
#include <SDL_ttf.h>
#include "Texture2D.h"
#include "TextureComponent.h"


namespace dae {
	class Font;
	class TextComponent : public TextureComponent
	{
	public:
		TextComponent(GameObject* gameObject, Font* font);
		TextComponent(GameObject* gameObject,const std::string& text, Font* font);

		void SetText(const std::string& text);
		void SetFont(Font* font);

		void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
		void SetColor(SDL_Color color);

		virtual void Update() override;
	private:
		bool m_Changed;
		std::string m_Text;
		Font* m_Font;
		std::unique_ptr<Texture2D> m_TextTexture{ };
		SDL_Color m_Color;
	};
}


