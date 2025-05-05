#pragma once
#include "SceneManager.h"

namespace Rinigin
{
	class Scene;
}
class Game;

class GameStateScene
{
public:
	explicit GameStateScene(Game* game,const std::string& sceneName);
	virtual ~GameStateScene() {};

	Rinigin::Scene* GetScene() const { return m_Scene; };
	Game* GetGame() const { return m_Game; };

	virtual void InitializeScene() = 0;
private:
	Game* m_Game;
	Rinigin::Scene* m_Scene;
};

