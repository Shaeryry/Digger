#pragma once
#include <memory>
#include <glm.hpp>
// Class for the digger game
namespace Rinigin { class Scene; class Gamepad; }

class DiggerGame final
{
public:
	explicit DiggerGame(float screenWidth,float screenHeight);
	glm::vec2 GetScreenSize() const { return { m_ScreenWidth,m_ScreenHeight }; }
private:
	void InitializeGame();

	int m_ScreenWidth;
	int m_ScreenHeight;
	Rinigin::Scene* m_GameScene;

	Rinigin::Gamepad* m_PlayerOne; // Keyboard
	Rinigin::Gamepad* m_PlayerTwo; // Controller
};

