#pragma once
#include "TextureComponent.h"


namespace dae {
	class Font;
	class TextComponent final : public TextureComponent
	{
	public:
		TextComponent(const std::string& text, std::shared_ptr<Font> font);
		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);

		virtual void Update() override;
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
	};
}


