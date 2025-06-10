#pragma once
#include <Component.h>
#include <string>

struct SDL_Color;
namespace Rinigin { class Texture2D; }
class LetterTextComponent : public Rinigin::Component
{
public:
	explicit LetterTextComponent(Rinigin::GameObject* gameObject,const char* text);

	void SetColor(const SDL_Color& color) { m_Color = color; }
	void SetText(const char* text) { m_Text = text; }
	void SetLetterSpacing(float spacing) { m_CharSpacing = spacing; }

	float GetLength() const;
	void Render() const override;
private:
	Rinigin::Texture2D* GetGlyph(size_t characterIndex) const;

	SDL_Color m_Color;
	std::string m_Text;
	float m_CharSpacing;
};

