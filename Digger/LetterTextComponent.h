#pragma once
#include <Component.h>
#include <string>
#include <SDL.h>

namespace Rinigin { class Texture2D; }
class LetterTextComponent : public Rinigin::Component
{
public:
	explicit LetterTextComponent(Rinigin::GameObject* gameObject,const char* text);

	void SetColor(const SDL_Color& color) { m_Color = color; }
	void SetSelectionColor(const SDL_Color& color) { m_SelectionColor = color; }
	void SetText(const char* text);
	void SetLetterSpacing(float spacing) { m_CharSpacing = spacing; }

	float GetLength() const;
	void Render() const override;
	void SelectLetter(int index) { m_SelectedLetters[index] = true; }
	void UnselectLetter(int index) { m_SelectedLetters[index] = false; }
private:
	Rinigin::Texture2D* GetGlyph(size_t characterIndex) const;
	std::vector<bool> m_SelectedLetters;
	SDL_Color m_Color;
	SDL_Color m_SelectionColor;
	std::string m_Text;
	float m_CharSpacing;
};

