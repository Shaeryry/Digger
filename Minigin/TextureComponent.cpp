#include "TextureComponent.h"
#include "GameObject.h"
#include "TextureRendererComponent.h"


dae::TextureComponent::TextureComponent(GameObject* gameObject, TextureRendererComponent* textureRenderer) :
	Component(gameObject),
	m_Renderer{ textureRenderer }
{
}
		
dae::TextureComponent::TextureComponent(GameObject* gameObject, TextureRendererComponent* textureRenderer, const std::string& filename) :
	TextureComponent(gameObject,textureRenderer)
{
	SetTexture(filename);
}

void dae::TextureComponent::SetTexture(const std::string& fileName)
{ 
	if (m_Renderer) m_Renderer->SetTexture(fileName);
}
