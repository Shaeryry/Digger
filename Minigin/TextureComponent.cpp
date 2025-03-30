#include "TextureComponent.h"
#include "GameObject.h"
#include "TextureRendererComponent.h"


Rinigin::TextureComponent::TextureComponent(GameObject* gameObject, TextureRendererComponent* textureRenderer) :
	Component(gameObject),
	m_Renderer{ textureRenderer }
{
}
		
Rinigin::TextureComponent::TextureComponent(GameObject* gameObject, TextureRendererComponent* textureRenderer, const std::string& filename) :
	TextureComponent(gameObject,textureRenderer)
{
	SetTexture(filename);
}

void Rinigin::TextureComponent::SetTexture(const std::string& fileName)
{ 
	if (m_Renderer) m_Renderer->SetTexture(fileName);
}
