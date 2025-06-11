#pragma once
#include "Component.h"

class LivesComponent : public Rinigin::Component
{
public:
	explicit LivesComponent(Rinigin::GameObject* gameObject,int lives);

	void AddLives(int lives);
	void RemoveLives(int lives = 1.f);
	void SetLives(int lives) { m_Lives = lives; }

	int GetLives() const { return m_Lives; }
private:
	int m_Lives;
};

