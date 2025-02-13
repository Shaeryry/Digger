#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"


dae::TextureComponent::TextureComponent(GameObject* gameObject) :
	Component(gameObject),
	m_Texture{ nullptr }
{
}

dae::TextureComponent::TextureComponent(GameObject* gameObject,const std::string& filename) :
	TextureComponent(gameObject)
{
	SetTexture(filename);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::SetTexture(Texture2D* texture)
{
	m_Texture = texture;
}

void dae::TextureComponent::Render() const
{
	if (m_Texture != nullptr) {
		const glm::vec3 position{ GetTransform()->GetPosition() };
		Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
	}
}
