#include "Item.h"
#include "SceneManager.h"
#include "Scene.h"

#include "ColliderComponent.h"
#include "SpriteSheetComponent.h"
#include "SpriteAnimatorComponent.h"
#include "TextureRendererComponent.h"
#include "TextureComponent.h"

#include "Level.h"

Item::Item(Level* level,const std::string& fileName) :
	m_Level(level),
	m_ItemObject(nullptr)
{
	m_ItemObject = level->GetScene()->CreateObject();
	m_ItemObject->SetActive(false);

	m_Renderer = m_ItemObject->AddComponent<Rinigin::TextureRendererComponent>();
	m_TextureComponent = m_ItemObject->AddComponent<Rinigin::TextureComponent>(m_Renderer);
	m_TextureComponent->SetTexture(fileName);
}
