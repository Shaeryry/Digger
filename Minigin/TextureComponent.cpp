#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"


dae::TextureComponent::TextureComponent(const std::string& filename)
{
	SetTexture(filename);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::Render() const
{
	if (m_texture != nullptr) {
		glm::vec3 position{ m_transform->GetPosition() };
		Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);
	}
}
