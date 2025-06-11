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
	m_Lives = newLives;
}
