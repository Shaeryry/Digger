#pragma once
#include <memory>
#include "GameStateScene.h"
// Class for the digger game
namespace Rinigin { class Scene; }

class Game final
{
public:
	explicit Game();
	void LoadScene(GameStateScene* gameStateScene);
private:
	std::unique_ptr<GameStateScene> m_MainMenuScene;
	std::unique_ptr<GameStateScene> m_GameScene;
};

