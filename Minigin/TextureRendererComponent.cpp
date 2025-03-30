#include "TextureRendererComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

Rinigin::TextureRendererComponent::TextureRendererComponent(GameObject* gameObject) :
	Component(gameObject),
	m_Texture{ nullptr }
{
}

void Rinigin::TextureRendererComponent::SetTexture(const std::string& filename)
{ 
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void Rinigin::TextureRendererComponent::SetTexture(Texture2D* texture)
{
	m_Texture = texture;
}

void Rinigin::TextureRendererComponent::Render() const
{
	if (m_Texture != nullptr) {
		const glm::vec3 position{ GetOwner()->GetWorldPosition() };
		Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
	}
}
