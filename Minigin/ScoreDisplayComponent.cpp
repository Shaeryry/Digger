#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* gameObject, TextComponent* textComponent) :
	Component(gameObject),
	m_TextComponent{ textComponent }
{
	UpdateScore(0);
}

void dae::ScoreDisplayComponent::Notify(EventType eventType, GameObject* gameObject)
{
	switch (eventType)
	{
	case dae::EventType::ScoreChanged:
		auto scoreComponent = gameObject->GetComponent<dae::ScoreComponent>();
		if (scoreComponent) {
			UpdateScore(scoreComponent->GetScore());
		}
		break;
	}
}

void dae::ScoreDisplayComponent::UpdateScore(int score)
{
	m_TextComponent->SetText("Score : " + std::to_string(score));
}
