#include "GameStateScene.h"

GameStateScene::GameStateScene(Game* game,const std::string& sceneName) :
	m_Game(game),
	m_Scene(Rinigin::SceneManager::GetInstance().CreateScene(sceneName) )
{
}
