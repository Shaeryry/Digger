#include "Character.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TextureRendererComponent.h"
#include "TextureComponent.h"
#include "SpriteSheetComponent.h"

Character::Character() :
	m_CharacterObject(Rinigin::SceneManager::GetInstance().GetActiveScene()->CreateObject()),
	m_Renderer(nullptr)
{
	m_CharacterObject->SetActive(false);
	m_Renderer = m_CharacterObject->AddComponent<Rinigin::TextureRendererComponent>();
	m_TextureComponent = m_CharacterObject->AddComponent<Rinigin::TextureComponent>(m_Renderer);
	m_SpriteSheetComponent = m_CharacterObject->AddComponent<Rinigin::SpriteSheetComponent>(m_Renderer,m_TextureComponent);
}