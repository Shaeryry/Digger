#pragma once

namespace Rinigin { 
	class GameObject; 
	class TextureRendererComponent; 
	class TextureComponent;
	class SpriteSheetComponent;
}

// TODO : Add ability to move this character with commands, maybe another interface !
class Character
{
public:
	Character();
	virtual ~Character() = default;

	Character(const Character& other) = delete;
	Character(Character&& other) = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) = delete;

	Rinigin::GameObject* GetCharacterObject() const { return m_CharacterObject; };
	Rinigin::TextureComponent* GetTextureComponent() const { return m_TextureComponent; }
	Rinigin::SpriteSheetComponent* GetSpriteSheetComponent() const { return m_SpriteSheetComponent; }
private:
	Rinigin::GameObject* m_CharacterObject;
	
	Rinigin::TextureRendererComponent* m_Renderer;
	Rinigin::TextureComponent* m_TextureComponent;
	Rinigin::SpriteSheetComponent* m_SpriteSheetComponent;
};

