#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"

class ScoreComponent final : public Rinigin::Component, public Rinigin::Observer
{
public: 
	explicit ScoreComponent(Rinigin::GameObject* gameObject);
	void virtual Notify(Rinigin::EventArguments* eventType) override;

	int GetScore() const { return m_Score; };
	Rinigin::Event* GetScoreChangedEvent() const { return m_ScoreChangedEvent.get(); };
private:
	void AddScore(int amount);

	std::unique_ptr<Rinigin::Event> m_ScoreChangedEvent;
	int m_Score;
};


