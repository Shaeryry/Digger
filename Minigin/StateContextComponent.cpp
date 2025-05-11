#include "StateContextComponent.h"
#include "State.h"

Rinigin::StateContextComponent::StateContextComponent(GameObject* gameObject) :
	Component(gameObject),
	m_Transitioning(false),
	m_CurrentState(nullptr)
{
}

bool Rinigin::StateContextComponent::SetState(State* state)
{
	if (state) {
		if (m_Transitioning) return false;
		if (state == m_CurrentState) return false;
		if (!HasState(state)) return false; // Check if this state is a part of this context

		m_Transitioning = true;

		if (m_CurrentState) m_CurrentState->Exit();
		state->Enter();
		m_CurrentState = state;

		m_Transitioning = false;
		return true;
	}

	return false;
}

void Rinigin::StateContextComponent::RemoveState(State* state)
{
	if (!HasState(state)) return; // Check if this state is a part of this context
}

void Rinigin::StateContextComponent::Update()
{
	if (m_CurrentState)	m_CurrentState->Update();
}

bool Rinigin::StateContextComponent::HasState(State* state)
{
	for (auto& otherState : m_States) {
		if (otherState.get() == state) return true;
	}

	return false;
}
