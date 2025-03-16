#include "Achievement.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include <iostream>
void dae::Achievement::Notify(EventType event, GameObject* gameObject)
{
	switch (event)
	{
	case dae::EventType::ScoreChanged:
		auto scoreComponent = gameObject->GetComponent<dae::ScoreComponent>();
		if (scoreComponent) {
			if (scoreComponent->GetScore() >= 500) {
				// TODO : ADD ACHIEVEMENT !
				std::cout << "ACHIEVEMENT UNLOCKED !" << std::endl;
			}
		}

		break;
	}
}
