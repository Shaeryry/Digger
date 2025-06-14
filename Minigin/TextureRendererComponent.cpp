#include "TextureRendererComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

Rinigin::TextureRendererComponent::TextureRendererComponent(GameObject* gameObject) :
	Component(gameObject),
	m_Texture(nullptr),
	m_UseSrcRect(false),
	m_Rect( SDL_Rect(0,0,0,0) ),
	m_SrcRect( SDL_Rect(0,0,0,0) )
{
}

void Rinigin::TextureRendererComponent::SetTexture(const std::string& filename)
{ 
	SetTexture(ResourceManager::GetInstance().LoadTexture(filename));
}

void Rinigin::TextureRendererComponent::SetTexture(Texture2D* texture)
{
	// Set the image sizes
	glm::vec2 imageSize = texture->GetSize();
	SetRectWidth(imageSize.x);
	SetRectHeight(imageSize.y);

	m_Texture = texture;
}

void Rinigin::TextureRendererComponent::Render() const
{
	if (m_Texture != nullptr) {
		const glm::vec3 position{ GetOwner()->GetWorldPosition() };

		SDL_Rect dstRect = m_Rect;
		dstRect.x += static_cast<int>(position.x);
		dstRect.y += static_cast<int>(position.y);

		if (m_UseSrcRect) {
			Renderer::GetInstance().RenderTexture(*m_Texture, dstRect, m_SrcRect);
		}
		else {
			Renderer::GetInstance().RenderTexture(*m_Texture, position.x,position.y);
		}
	}
}
