#pragma once
#include <vector>
#include <memory>
#include "Component.h"

namespace Rinigin
{
	class State;
	class StateContextComponent final : public Component {
	public:
		explicit StateContextComponent(GameObject* gameObject);

		bool SetState(State* state);
		template <typename StateType,typename... Arguments> StateType* CreateState(Arguments&&... args);
		template <typename StateType> State* GetState() const;

		void RemoveState(State* state);
		State* GetCurrentState() const { return m_CurrentState; };

		void Update() override;
	private:
		bool HasState(State* state);
		 
		std::vector< std::unique_ptr<State> > m_States;
		State* m_CurrentState;
		bool m_Transitioning;
	};  

	template<typename StateType,typename... Arguments>
	inline StateType* StateContextComponent::CreateState(Arguments&&... args)
	{
		std::unique_ptr<StateType> createdState{ std::make_unique<StateType>(this, std::forward<Arguments>(args)... ) };
		m_States.emplace_back( std::move(createdState) );
		return dynamic_cast<StateType*>( m_States.back().get() );
	}

	template<typename StateType>
	inline State* StateContextComponent::GetState() const
	{
		for (const auto& state : m_States)
		{
			StateType* foundState = dynamic_cast<StateType*>(state.get());
			if (dynamic_cast<StateType*>(state.get()))
			{
				return foundState;
			}
		}
	}

}


