#pragma once
#include <string>

namespace Rinigin { 
	class GameObject;
	class TextureRendererComponent; 
	class TextureComponent;
	class SpriteSheetComponent;
	class SpriteAnimatorComponent;
	class ColliderComponent;
	class RigidbodyComponent;
}

class HealthComponent;

class DamageCommand;
class MovementCommand;
class ChangeDirectionCommand;
// TODO : Add ability to move this character with commands, maybe another interface !
class Character
{ 
public:
	Character(const std::string& fileName);
	virtual ~Character() = default;
	Character(const Character& other) = delete;
	Character(Character&& other) = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) = delete;

	Rinigin::GameObject* GetCharacterObject() const { return m_CharacterObject; };
	Rinigin::SpriteAnimatorComponent* GetAnimator() const { return m_Animator; };
	Rinigin::TextureComponent* GetTextureComponent() const { return m_TextureComponent; }
	Rinigin::SpriteSheetComponent* GetSpriteSheetComponent() const { return m_SpriteSheetComponent; };
	HealthComponent* GetHealthComponent() const { return m_HealthComponent; };

	DamageCommand* GetDamageCommand() const { return m_DamageCommand; } // TEMPORARY WAY TO DO DAMAGE????
	MovementCommand* GetMoveCommand() const { return m_MoveCommand; };
	ChangeDirectionCommand* UpDirectionCommand() const { return m_UpDirectionCommand; }
	ChangeDirectionCommand* DownDirectionCommand() const { return m_DownDirectionCommand; }
	ChangeDirectionCommand* RightDirectionCommand() const { return m_RightDirectionCommand; }
	ChangeDirectionCommand* LeftDirectionCommand() const { return m_LeftDirectionCommand; }

	Rinigin::ColliderComponent* GetCollider() const { return m_Collider; };
	Rinigin::ColliderComponent* GetTrigger() const { return m_Trigger; };

	Rinigin::RigidbodyComponent* GetRigidbody() const { return m_Rigidbody; }
	void SetSpeed(float speed);
private:
	Rinigin::GameObject* m_CharacterObject;
	
	Rinigin::ColliderComponent* m_Collider;
	Rinigin::ColliderComponent* m_Trigger;

	Rinigin::RigidbodyComponent* m_Rigidbody;
	Rinigin::TextureRendererComponent* m_Renderer;
	Rinigin::TextureComponent* m_TextureComponent;
	Rinigin::SpriteSheetComponent* m_SpriteSheetComponent;
	Rinigin::SpriteAnimatorComponent* m_Animator;
	HealthComponent* m_HealthComponent;

	DamageCommand* m_DamageCommand;
	ChangeDirectionCommand* m_UpDirectionCommand;
	ChangeDirectionCommand* m_DownDirectionCommand;
	ChangeDirectionCommand* m_RightDirectionCommand;
	ChangeDirectionCommand* m_LeftDirectionCommand;
	MovementCommand* m_MoveCommand;

	float m_Speed;
};