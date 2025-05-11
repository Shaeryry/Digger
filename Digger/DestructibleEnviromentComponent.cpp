#include "DestructibleEnviromentComponent.h"
#include "Renderer.h"

DestructibleEnvironmentComponent::DestructibleEnvironmentComponent(Rinigin::GameObject* gameObject, int mapWidth, int mapHeight) :
	Component(gameObject),
	m_Width(mapWidth),
	m_Height(mapWidth),
	m_MapPixels( new uint32_t[mapWidth * mapHeight] )  
{ 
	m_MapTexture = SDL_CreateTexture(Rinigin::Renderer::GetInstance().GetSDLRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, mapWidth, mapHeight);
	SDL_SetTextureBlendMode(m_MapTexture, SDL_BLENDMODE_BLEND);

	uint32_t dirtColor = 0xFF8B4513;
	for (int i = 0; i < mapWidth * mapHeight; ++i) m_MapPixels[i] = dirtColor;
} 

DestructibleEnvironmentComponent::~DestructibleEnvironmentComponent()
{
	delete[] m_MapPixels;
}

void DestructibleEnvironmentComponent::Update()
{

} 

void DestructibleEnvironmentComponent::Render() const
{

}
