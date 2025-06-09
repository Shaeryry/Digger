#pragma once
#include "Component.h"
#include "ICollisionMask.h"
#include <glm.hpp>

namespace Rinigin { class GameObject; }

struct SDL_Texture;
class TerrainComponent : public Rinigin::Component,public Rinigin::ICollisionMask
{
public:
	explicit TerrainComponent(Rinigin::GameObject* gameObject,const glm::vec2& screenSize, const glm::vec2& mapSize);
	~TerrainComponent();

	void ChangeBackgroundTexture(const char* filePath);

	void DigAt(float cx, float cy, int radius, bool square = false);
	void DigAt(int cx, int cy, int radius, bool square = false);
	void Update() override; 
	void Render() const override;

	virtual bool IsSolidAt(int x, int y) const override;
	void ResetPixels();

	int GetMapWidth() const { return m_Width; }
	int GetMapHeight() const { return m_Height; }
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

