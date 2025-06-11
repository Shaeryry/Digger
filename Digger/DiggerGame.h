#pragma once
#include <memory>
#include <glm.hpp>
// Class for the digger game
namespace Rinigin { class Scene; class Gamepad; struct Binding; }

class MenuCommand;
class DiggerGame final
{
public:
	explicit DiggerGame(float screenWidth,float screenHeight);
	glm::vec2 GetScreenSize() const { return { m_ScreenWidth,m_ScreenHeight }; }
private:
	void InitializeGame();
	void RegisterBindings();

	int m_ScreenWidth;
	int m_ScreenHeight;
	Rinigin::Scene* m_GameScene;

	Rinigin::Gamepad* m_PlayerOne; // Keyboard
	Rinigin::Gamepad* m_PlayerTwo; // Controller

	// Commands
	MenuCommand* m_MenuMoveUpCommand;
	MenuCommand* m_MenuMoveDownCommand;
	MenuCommand* m_MenuMoveLeftCommand;
	MenuCommand* m_MenuMoveRightCommand;
	MenuCommand* m_MenuConfirmCommand;

	// Bindings

	Rinigin::Binding* m_SelectUpKeyboard;
	Rinigin::Binding* m_SelectDownKeyboard;
	Rinigin::Binding* m_SelectLeftKeyboard;
	Rinigin::Binding* m_SelectRightKeyboard;
	Rinigin::Binding* m_ConfirmKeyboard;

	Rinigin::Binding* m_SelectUpGamepad;
	Rinigin::Binding* m_SelectDownGamepad;
	Rinigin::Binding* m_SelectLeftGamepad;
	Rinigin::Binding* m_SelectRightGamepad;
	Rinigin::Binding* m_ConfirmGamepad;
};

