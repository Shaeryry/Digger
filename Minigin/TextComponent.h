#pragma once
#include "TextureComponent.h"


namespace dae {
	class Font;
	class TextComponent : public TextureComponent
	{
	public:
		TextComponent(GameObject* gameObject, std::shared_ptr<Font> font);
		TextComponent(GameObject* gameObject,const std::string& text, std::shared_ptr<Font> font);
		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);

		virtual void Update() override;
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
	};
}


