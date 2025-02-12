#pragma once
#include <memory>
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

		virtual void Update() override;
	private:
		bool m_Changed;
		std::string m_Text;
		Font* m_Font;
		std::unique_ptr<Texture2D> m_TextTexture{ };
	};
}


