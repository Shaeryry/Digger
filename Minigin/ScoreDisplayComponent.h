#pragma once
#include "Component.h"
#include "Observer.h"

namespace Rinigin { class TextComponent; };
class ScoreDisplayComponent : public Rinigin::Component, public Rinigin::Observer
{
public:
	explicit ScoreDisplayComponent(Rinigin::GameObject* gameObject,Rinigin::TextComponent* textComponent);
	virtual void Notify(Rinigin::EventArguments & eventArguments) override;
private:
	void UpdateScore(int score);
	Rinigin::TextComponent* m_TextComponent;
};



