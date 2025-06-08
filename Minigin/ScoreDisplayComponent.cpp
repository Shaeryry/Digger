#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "EventTypes.h"
#include "Helpers.h"

ScoreDisplayComponent::ScoreDisplayComponent(Rinigin::GameObject* gameObject, Rinigin::TextComponent* textComponent) :
	Component(gameObject),
	m_TextComponent{ textComponent }
{
	UpdateScore(0);
}

void ScoreDisplayComponent::Notify(Rinigin::EventArguments & /*eventArguments*/ /*eventArguments*/)
{
	/*switch (eventArguments.GetID())
	{
		case Rinigin::Helpers::sdbm_hash("ScoreChanged"): {
			auto arguments = dynamic_cast<GameObjectEventArguments*>(eventArgs);
			auto scoreComponent = arguments->GetGameObject()->GetComponent<ScoreComponent>();
			if (scoreComponent) {
				UpdateScore(scoreComponent->GetScore());
			}
			break;
		}
	}*/
}

void ScoreDisplayComponent::UpdateScore(int score)
{
	m_TextComponent->SetText("Score : " + std::to_string(score));
}
