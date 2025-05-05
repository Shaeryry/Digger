#pragma once
#include <memory>
#include <vector>
#include "Singleton.h"
#include "Gamepad.h"
#include "Command.h"

namespace Rinigin
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		void Initialize(int amountGamepads = 1);
		bool ProcessInput();	
		
		Gamepad* GetGamepad(int index) { return m_Gamepads[index].get(); };
		template<typename CommandType, typename... Arguments> CommandType* AddCommand(Arguments&&... args);
	private:
		Gamepad* AddGamepad(const int index);
		 
		std::vector<std::unique_ptr<Command>> m_Commands; // List of all commands present
		std::vector<std::unique_ptr<Gamepad>> m_Gamepads;
		Gamepad* m_Keyboard; 
	};
	 
	template<typename CommandType, typename... Arguments>
	inline CommandType* InputManager::AddCommand(Arguments&&... args)
	{
		std::unique_ptr<CommandType> newCommand{ std::make_unique<CommandType>( std::forward<Arguments>(args)... ) };
		m_Commands.emplace_back( std::move(newCommand) );
		return dynamic_cast<CommandType*>( m_Commands.back().get() );
	}
}
