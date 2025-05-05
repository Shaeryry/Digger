#pragma once
#include "GameStateScene.h"

class InGameState final : public GameStateScene
{
public:
	explicit InGameState(Game* game);
	~InGameState() override {};
	void InitializeScene() override;
private:

};

