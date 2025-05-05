#pragma once

namespace Rinigin
{
	class StateContextComponent;
	class State
	{
	public:
		explicit State(StateContextComponent* context) : m_Context(context) {};
		virtual ~State() = default;
		State(const State& other) = delete;
		State(State&& other) = delete;
		State& operator=(const State& other) = delete;
		State& operator=(State&& other) = delete;

		virtual void Enter() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;
	private:
		StateContextComponent* m_Context;
	};
}
