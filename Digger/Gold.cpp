#include "Gold.h"
#include "GameObject.h"
#include "ColliderComponent.h"

#include "SpriteAnimatorComponent.h"
#include "SpriteSheetComponent.h"

#include "EventTypes.h"

#include "Level.h"
#include "ScoreComponent.h"
#include "Character.h"
#include "PrototypeSpawner.h"

Gold::Gold(Level* level) : 
	Item(level, "Gold.png"),
	m_CollectEvent(std::make_unique<Rinigin::Event>()),
	m_Trigger(nullptr)
{
	m_Trigger = GetItemObject()->AddComponent<Rinigin::ColliderComponent>(glm::vec3{ 32,30,0 }, glm::vec3{ 0,0,0 }, true);
	m_Trigger->ColliderEnterEvent()->AddObserver(this);

	m_SpriteSheetComponent = GetItemObject()->AddComponent<Rinigin::SpriteSheetComponent>(GetRenderer(), GetTextureComponent());
	m_Animator = GetItemObject()->AddComponent<Rinigin::SpriteAnimatorComponent>(m_SpriteSheetComponent);

	// Sprite sheet
	m_SpriteSheetComponent->SetDisplayMode(Rinigin::SpriteDisplayMode::Tile);
	m_SpriteSheetComponent->SetRows(3);
	m_SpriteSheetComponent->SetColumns(1);

	// Animation
	m_Animator->AddAnimation("Gold", Rinigin::SpriteAnimationData{ 0,0,3,.5f,false });
	m_Animator->PlayAnimation("Gold");
}

void Gold::Notify(Rinigin::EventArguments& eventArguments)
{
	switch (eventArguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("CollisionEnter"): {
		Rinigin::CollisionEventArguments& collisionArgument{ GetArgumentsOfType<Rinigin::CollisionEventArguments>(eventArguments) };
		const unsigned int layerId{ collisionArgument.GetOther()->GetCollisionLayer() };
		const unsigned int playerLayerId{ Rinigin::Helpers::sdbm_hash("Player") };
		if (layerId == playerLayerId) {
			GameObjectEventArguments arguments{ "GoldCollected",collisionArgument.GetOther()->GetOwner() };
			m_CollectEvent->NotifyObservers(arguments);
			GetItemObject()->Destroy();
			m_Level->GetItemSpawner().RemoveTracked(this);
		};

		break;
	}
	default:
		break;
	}
}

Item* Gold::Clone()
{
	Gold* newGold = new Gold(m_Level);
	newGold->GetItemObject()->SetActive(true);

	for (Character* character : m_Level->GetPlayers()) {
		ScoreComponent* scoreComponent = character->GetCharacterObject()->GetComponent<ScoreComponent>();
		if (scoreComponent) {
			newGold->GetCollectedEvent()->AddObserver(scoreComponent);
		}
	}

	return newGold;
}
