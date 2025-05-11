#pragma once
#include "Component.h"

namespace Rinigin { class GameObject; }

struct SDL_Texture;
class DestructibleEnvironmentComponent : public Rinigin::Component
{
public:
	explicit DestructibleEnvironmentComponent(Rinigin::GameObject* gameObject,int mapWidth,int mapHeight);
	~DestructibleEnvironmentComponent();
	void Update() override;
	void Render() const override;
private:
	SDL_Texture* m_MapTexture;
	uint32_t* m_MapPixels;
	int m_Width;
	int m_Height;
};

