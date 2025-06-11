#include "LivesDisplayComponent.h"
#include "LivesComponent.h"

#include "ResourceManager.h"
#include "Renderer.h"

#include "GameObject.h"

LivesDisplayComponent::LivesDisplayComponent(Rinigin::GameObject* gameObject, LivesComponent* livesComponent) :
	Component(gameObject),
	m_LivesComponent(livesComponent)
{
}

void LivesDisplayComponent::Render() const
{
	const glm::vec3& position = GetOwner()->GetWorldPosition();
	Rinigin::Texture2D* lifeTexture{ Rinigin::ResourceManager::GetInstance().LoadTexture("Life.png") };

	for (int lifeIndex{ 0 }; lifeIndex < (m_LivesComponent->GetLives() - 1); lifeIndex++) {
		Rinigin::Renderer::GetInstance().RenderTexture(*lifeTexture, position.x + (lifeTexture->GetSize().x * lifeIndex), position.y);
	}
}
