#pragma once
#include "Component.h"
#include <glm.hpp>

namespace Rinigin { class GameObject; }

struct SDL_Texture;
class DestructibleEnvironmentComponent : public Rinigin::Component
{
public:
	explicit DestructibleEnvironmentComponent(Rinigin::GameObject* gameObject,const glm::vec2& screenSize, const glm::vec2& mapSize,int tileSize);
	~DestructibleEnvironmentComponent();

	void ChangeBackgroundTexture(const char* filePath);

	void DigAt(float cx, float cy, int radius);
	void Update() override; 
	void Render() const override;

	void ResetPixels();
private: 
	void UnlockMask();
	void LockMask();
	
	SDL_Texture* m_MaskTexture;
	uint32_t* m_MaskPixels;

	int m_BackgroundPitch;
	void* m_BackgroundPixels; 
	uint32_t* m_BackgroundPixelPtr;
	SDL_Texture* m_BackgroundTilesTexture;

	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_Width;
	int m_Height;
	int m_TileSize;
};

