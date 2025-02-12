#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"


dae::TextureComponent::TextureComponent(GameObject* gameObject) :
	Component(gameObject),
	m_texture{ nullptr }
{
}

dae::TextureComponent::TextureComponent(GameObject* gameObject,const std::string& filename) :
	TextureComponent(gameObject)
{
	SetTexture(filename);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_texture = std::move(texture);
}

void dae::TextureComponent::Render() const
{
	if (m_texture != nullptr) {
		glm::vec3 position{ GetTransform()->GetPosition()};
		Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);
	}
}
