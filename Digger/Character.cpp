#include "Character.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TextureRendererComponent.h"
#include "TextureComponent.h"
#include "SpriteSheetComponent.h"
#include "SpriteAnimatorComponent.h"
#include "MovementCommands.h"
#include "InputManager.h"

Character::Character() : 
	m_CharacterObject(Rinigin::SceneManager::GetInstance().GetActiveScene()->CreateObject()),
	m_Renderer(nullptr),
	m_TextureComponent(nullptr),
	m_SpriteSheetComponent(nullptr),
	m_Animator(nullptr), 

	m_MoveCommand(nullptr)
{
	m_CharacterObject->SetActive(false);

	m_Renderer = m_CharacterObject->AddComponent<Rinigin::TextureRendererComponent>();
	m_TextureComponent = m_CharacterObject->AddComponent<Rinigin::TextureComponent>(m_Renderer);
	m_SpriteSheetComponent = m_CharacterObject->AddComponent<Rinigin::SpriteSheetComponent>(m_Renderer,m_TextureComponent);
	m_Animator = m_CharacterObject->AddComponent<Rinigin::SpriteAnimatorComponent>(m_SpriteSheetComponent);

	m_MoveCommand = Rinigin::InputManager::GetInstance().AddCommand<MovementCommand>(m_CharacterObject,glm::vec3(0, 1, 0),m_Speed);
	m_UpDirectionCommand = Rinigin::InputManager::GetInstance().AddCommand<ChangeDirectionCommand>(m_MoveCommand, glm::vec3(0, -1, 0));
	m_DownDirectionCommand = Rinigin::InputManager::GetInstance().AddCommand<ChangeDirectionCommand>(m_MoveCommand, glm::vec3(0, 1, 0));
	m_RightDirectionCommand = Rinigin::InputManager::GetInstance().AddCommand<ChangeDirectionCommand>(m_MoveCommand, glm::vec3(1, 0, 0));
	m_LeftDirectionCommand = Rinigin::InputManager::GetInstance().AddCommand<ChangeDirectionCommand>(m_MoveCommand, glm::vec3(-1, 0, 0));
}

void Character::SetSpeed(float speed)
{
	m_MoveCommand->SetSpeed(speed);
	m_Speed = speed;
}
