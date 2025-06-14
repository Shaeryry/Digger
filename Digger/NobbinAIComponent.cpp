#include "NobbinAIComponent.h"
#include "Pathfinder.h"
#include "Level.h"
#include "TerrainComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Character.h"
#include "ColliderComponent.h"
#include "Timer.h"
#include "MovementCommands.h"
#include "Nobbin.h"
#include "RigidbodyComponent.h"
#include "DiggerConstants.h"
#include <iostream>
#include "NobbinPursueState.h"
#include "NobbinIdleState.h"

#include "StateContextComponent.h"

NobbinAIComponent::NobbinAIComponent(Rinigin::GameObject* gameObject, Nobbin* nobbin) :
	Component(gameObject),
	m_Nobbin(nobbin)
{
}

void NobbinAIComponent::Update()
{
	Character* closestCharacter = FindClosestPlayer();
	if (closestCharacter) {
		glm::vec3 targetPosition = closestCharacter->GetCenteredPosition();

		m_Nobbin->GetPursueState()->SetTarget(targetPosition);
		m_Nobbin->GetStateContext()->SetState(m_Nobbin->GetPursueState());
	}
	else {
		m_Nobbin->GetStateContext()->SetState(m_Nobbin->GetIdleState());
	}
}

void NobbinAIComponent::Render() const
{
	//m_Nobbin->GetPursueState()->DrawPath();
}

Character* NobbinAIComponent::FindClosestPlayer() const
{
	Level* level = m_Nobbin->GetLevel();
	if (!level) return nullptr;

	const auto& deadPlayers = level->GetDeadPlayers();
	const auto& players = level->GetPlayers();
	if (players.empty()) return nullptr;

	glm::vec3 myPos = GetOwner()->GetWorldPosition();

	auto isValid = [deadPlayers](Character* c) {
		return c &&
			std::find(deadPlayers.begin(), deadPlayers.end(),c) == deadPlayers.end() and
			c->GetCharacterObject() and
			c->GetCharacterObject()->IsActive();
		};

	auto closestIt = std::min_element(players.begin(), players.end(),
		[&](Character* a, Character* b) {
			const bool validA = isValid(a);
			const bool validB = isValid(b);

			if (!validA && !validB) return false;
			if (!validA) return false;
			if (!validB) return true;

			float distA = glm::distance(myPos, a->GetCharacterObject()->GetWorldPosition());
			float distB = glm::distance(myPos, b->GetCharacterObject()->GetWorldPosition());
			return distA < distB;
		});

	if (closestIt != players.end() && isValid(*closestIt)) {
		return *closestIt;
	}

	return nullptr;
}



