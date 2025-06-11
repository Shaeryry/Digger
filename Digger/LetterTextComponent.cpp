#include "LetterTextComponent.h"
#include <glm/glm.hpp>

#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ResourceManager.h"

#include <iostream>

LetterTextComponent::LetterTextComponent(Rinigin::GameObject* gameObject, const char* text) :
	Component(gameObject),
    m_CharSpacing(0)
{
    SetColor({ 255,255,255,255 });
	SetText(text);
}

float LetterTextComponent::GetLength() const
{
    float size{ 0 };
    for (size_t characterIndex{ 0 }; characterIndex < m_Text.size(); characterIndex++) {
        Rinigin::Texture2D* letterTexture{ GetGlyph(characterIndex) };
        size += letterTexture->GetSize().x + m_CharSpacing;
    }

    return size;
}

void LetterTextComponent::Render() const
{
    const glm::vec3& position = GetOwner()->GetWorldPosition();
    float cursorX = position.x;

    for (size_t characterIndex{ 0 }; characterIndex < m_Text.size(); characterIndex++) {
        Rinigin::Texture2D* letterTexture{ GetGlyph(characterIndex) };
        Rinigin::Renderer::GetInstance().RenderTexture(*letterTexture, cursorX, position.y,m_Color);

        cursorX += letterTexture->GetSize().x + m_CharSpacing;
    }
}

Rinigin::Texture2D* LetterTextComponent::GetGlyph(size_t characterIndex) const
{
    std::string character = std::string{ m_Text[characterIndex] };
    if (character == ".") character = "DOT";
    if (character == ":") character = "SEMICOLON";
    if (character == "_") character = "UNDERSCORE";

    const std::string filePath{ std::string("Letters/") + character + std::string(".BMP") };
    Rinigin::Texture2D* letterTexture{ Rinigin::ResourceManager::GetInstance().LoadTexture(filePath) };
    return letterTexture;
}
