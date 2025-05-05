#include "TextureRendererComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

Rinigin::TextureRendererComponent::TextureRendererComponent(GameObject* gameObject) :
	Component(gameObject),
	m_Texture(nullptr),
	m_Rect( RendererRect(0,0,0,0) ),
	m_SrcRect( RendererRect(0,0,0,0) )
{
}

void Rinigin::TextureRendererComponent::SetTexture(const std::string& filename)
{ 
	SetTexture(ResourceManager::GetInstance().LoadTexture(filename));
}

void Rinigin::TextureRendererComponent::SetTexture(Texture2D* texture)
{
	m_Texture = texture;
	
	// Set the image sizes
	glm::vec2 imageSize = m_Texture->GetSize();
	SetWidth(imageSize.x);
	SetHeight(imageSize.y);
}

void Rinigin::TextureRendererComponent::Render() const
{
	if (m_Texture != nullptr) {
		const glm::vec3 position{ GetOwner()->GetWorldPosition()  };
		Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y,m_Rect.w,m_Rect.h);
	}
}
