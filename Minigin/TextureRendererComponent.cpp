#include "TextureRendererComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

dae::TextureRendererComponent::TextureRendererComponent(GameObject* gameObject) :
	Component(gameObject),
	m_Texture{ nullptr }
{
}

void dae::TextureRendererComponent::SetTexture(const std::string& filename)
{ 
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureRendererComponent::SetTexture(Texture2D* texture)
{
	m_Texture = texture;
}

void dae::TextureRendererComponent::Render() const
{
	if (m_Texture != nullptr) {
		const glm::vec3 position{ GetOwner()->GetWorldPosition() };
		Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
	}
}
