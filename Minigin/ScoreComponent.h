#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"

class ScoreComponent final : public Rinigin::Component, public Rinigin::Observer
{
public: 
	explicit ScoreComponent(Rinigin::GameObject* gameObject);
	virtual void Notify(Rinigin::EventArguments & eventArguments) override;

	int GetScore() const { return m_Score; };
	Rinigin::Event* GetScoreChangedEvent() const { return m_ScoreChangedEvent.get(); };
	Rinigin::Event* GetScoreAddedEvent() const { return m_ScoreAddedEvent.get(); };
private:
	void AddScore(int amount);
	std::unique_ptr<Rinigin::Event> m_ScoreAddedEvent;
	std::unique_ptr<Rinigin::Event> m_ScoreChangedEvent;

	int m_Score;
};


