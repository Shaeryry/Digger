#pragma once
#include "GameStateScene.h"

class MainMenuState final : public GameStateScene
{
public:
	explicit MainMenuState(Game* game);
	~MainMenuState() override {};
	void InitializeScene() override;
private:
	
};

