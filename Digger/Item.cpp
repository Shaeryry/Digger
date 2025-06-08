#include "Item.h"
#include "SceneManager.h"
#include "Scene.h"

#include "ColliderComponent.h"
#include "SpriteSheetComponent.h"
#include "SpriteAnimatorComponent.h"
#include "TextureRendererComponent.h"
#include "TextureComponent.h"

Item::Item(const std::string& fileName) :
	m_ItemObject(Rinigin::SceneManager::GetInstance().GetActiveScene()->CreateObject()),
	m_Trigger(nullptr)
{
	m_Trigger = m_ItemObject->AddComponent<Rinigin::ColliderComponent>(glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,0 }, true);
	m_Renderer = m_ItemObject->AddComponent<Rinigin::TextureRendererComponent>();
	m_TextureComponent = m_ItemObject->AddComponent<Rinigin::TextureComponent>(m_Renderer);
	m_TextureComponent->SetTexture(fileName);
	m_Trigger->SetLayer("Item");
}
