#include "TerrainComponent.h"
#include "Renderer.h"
#include <glm.hpp>
#include <SDL_image.h> 
#include "ResourceManager.h"

TerrainComponent::TerrainComponent(Rinigin::GameObject* gameObject,const glm::vec2& origin ,const glm::vec2& screenSize, const glm::vec2& mapSize) :
	Component(gameObject),
	m_Origin(origin)
{ 
	m_ScreenWidth = static_cast<int>(screenSize.x);
	m_ScreenHeight = static_cast<int>(screenSize.y);
	m_Width = static_cast<int>(mapSize.x);
	m_Height = static_cast<int>(mapSize.y);

	// Background tiles
	m_BackgroundTilesTexture = std::make_unique<Rinigin::Texture2D>( SDL_CreateTexture(
			Rinigin::Renderer::GetInstance().GetSDLRenderer(),
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			m_ScreenWidth,
			m_ScreenHeight
		) 
	);
	SDL_SetTextureBlendMode(m_BackgroundTilesTexture->GetSDLTexture(), SDL_BLENDMODE_NONE);

	m_BackgroundPixels = nullptr;
	m_BackgroundPitch = 0;
	//

	m_MaskTexture = std::make_unique<Rinigin::Texture2D>( SDL_CreateTexture(
			Rinigin::Renderer::GetInstance().GetSDLRenderer(),
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			m_ScreenWidth,
			m_ScreenHeight
		)
	);

	SDL_SetTextureBlendMode(m_MaskTexture->GetSDLTexture(), SDL_BLENDMODE_BLEND);

	m_MaskPixels = new Uint32[m_ScreenWidth * m_ScreenHeight];
	ResetPixels();
} 

TerrainComponent::~TerrainComponent()
{
	//delete[] m_MaskPixels;
}

void TerrainComponent::UnlockMask()
{
	SDL_UnlockTexture(m_BackgroundTilesTexture->GetSDLTexture());
}

void TerrainComponent::LockMask()
{
	SDL_LockTexture(m_BackgroundTilesTexture->GetSDLTexture(), nullptr, &m_BackgroundPixels, &m_BackgroundPitch);
	m_BackgroundPixelPtr = static_cast<uint32_t*>(m_BackgroundPixels);
}


void TerrainComponent::ChangeBackgroundTexture(const char* filePath)
{
	if (!filePath) return;

	LockMask();

	// Get data from the new texture
	SDL_Surface* loadedTexture = IMG_Load(filePath);
	// Convert the surface to the format you expect (ARGB8888)
	SDL_Surface* textureSurface = SDL_ConvertSurfaceFormat(loadedTexture, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(loadedTexture); // Free the original surface after conversion
	/*void* backgroundPixels = nullptr;
	int pitch = 0;
	SDL_LockTexture(texture->GetSDLTexture(), nullptr, &backgroundPixels, &pitch);*/

	const int size_x = textureSurface->w;
	const int size_y = textureSurface->h;

	// Fill the background texture with repeated tiles
	Uint32* tilePixels = static_cast<Uint32*>(textureSurface->pixels);
	int tilePitch = (textureSurface->pitch / 4);

	const int originX = static_cast<int>(m_Origin.x);
	const int originY = static_cast<int>(m_Origin.y);

	for (int y = originY; y < (m_Height + originY); y++) {
		for (int x = originX; x < (m_Width + originX); x++) {
			int tileX = x % size_x;
			int tileY = y % size_y;

			Uint32 color = tilePixels[tileY * tilePitch + tileX];
			m_BackgroundPixelPtr[y * (m_BackgroundPitch / 4) + x] = color;
		}
	}

	//SDL_UnlockTexture(m_BackgroundTexture->GetSDLTexture());
	UnlockMask();
	SDL_FreeSurface(textureSurface);
}

void TerrainComponent::DigAt(float cx, float cy, int radius, bool square)
{
	DigAt(static_cast<int>(cx), static_cast<int>(cy), radius,square);
}

void TerrainComponent::DigAt(int cx, int cy, int radius, bool square)
{

	for (int y = -radius; y <= radius; ++y) {
		for (int x = -radius; x <= radius; ++x) {
			int dx = cx + x;
			int dy = cy + y;

			if (dx >= 0 && dx < m_ScreenWidth && dy >= 0 && dy < m_ScreenHeight) {
				if (x * x + y * y <= radius * radius or square) {
					int index = dy * m_ScreenWidth + dx;
					m_MaskPixels[index] = 0x00000000;//0x00FFFFFF; // Set alpha to 0
				}
			}
		}
	}
}

void TerrainComponent::Update()
{
	SDL_UpdateTexture(m_MaskTexture->GetSDLTexture(), nullptr, m_MaskPixels, m_ScreenWidth * sizeof(Uint32));

	LockMask();

	const int originX = static_cast<int>(m_Origin.x);
	const int originY = static_cast<int>(m_Origin.y);

	for (int y = originY; y < (m_Height + originY); y++) {
		for (int x = originX; x < (m_Width + originX); x++) {
			// Check if the corresponding pixel in the mask texture is transparent
			Uint32 maskColor = m_MaskPixels[y * m_ScreenWidth + x];
			if ((maskColor & 0xFF000000) == 0) {  // If alpha == 0 (transparent)
				m_BackgroundPixelPtr[y * (m_BackgroundPitch / 4) + x] = 0x00000000;  // Make it transparent in the dirt texture
			}
		}
	}

	UnlockMask();
} 

void TerrainComponent::Render() const
{
	SDL_Renderer* renderer = Rinigin::Renderer::GetInstance().GetSDLRenderer();
	// Apply the destruction mask on top
	SDL_RenderCopy(renderer, m_BackgroundTilesTexture->GetSDLTexture(), nullptr, nullptr);
	//SDL_RenderPresent(renderer);
}

bool TerrainComponent::IsSolidAt(int x, int y) const 
{
	if (x < m_Origin.x or x >= (m_Width + m_Origin.x) or y < m_Origin.y or y >= (m_Height + m_Origin.y)) return false;

	Uint32 maskColor = m_MaskPixels[y * m_Width + x];
	return (maskColor & 0xFF000000) != 0; // Alpha != 0 means solid
}

void TerrainComponent::ResetPixels()
{
	for (int pixelIndex{ 0 }; pixelIndex < (m_ScreenWidth * m_ScreenHeight); pixelIndex++) {
		m_MaskPixels[pixelIndex] = 0xFFFFFFFF;
	}
}
