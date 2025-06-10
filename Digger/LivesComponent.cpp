#include "LivesComponent.h"

LivesComponent::LivesComponent(Rinigin::GameObject* gameObject, int lives) :
	Component(gameObject),
	m_Lives(lives)
{
}

void LivesComponent::AddLives(int lives)
{
	if (lives < 0) return;
	m_Lives += lives;
}

void LivesComponent::RemoveLives(int lives)
{
	int newLives = (m_Lives - lives);
	if (newLives <= 0) {

	}

	m_Lives = newLives;
}
