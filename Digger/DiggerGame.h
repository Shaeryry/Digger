#pragma once
#include <memory>
// Class for the digger game
namespace Rinigin { class Scene; class Gamepad; }

class DiggerGame final
{
public:
	explicit DiggerGame();
private:
	Rinigin::Scene* m_GameScene;

	Rinigin::Gamepad* m_PlayerOne; // Keyboard
	Rinigin::Gamepad* m_PlayerTwo; // Controller
};

