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
	m_Nobbin->GetPursueState()->DrawPath();
}

Character* NobbinAIComponent::FindClosestPlayer() const
{
	Character* closest = nullptr;
	float minDist = std::numeric_limits<float>::max();
	glm::vec3 myPos = GetOwner()->GetWorldPosition();
	Level* level = m_Nobbin->GetLevel();

	if (level) {
		for (int i = 0; i < m_Nobbin->GetLevel()->GetPlayers().size(); i++) {
			Character* player = m_Nobbin->GetLevel()->GetPlayer(i);

			if (!player or !player->GetCharacterObject()->IsActive()) continue;

			glm::vec3 playerPos = player->GetCharacterObject()->GetWorldPosition();
			float dist = glm::distance(myPos, playerPos);

			if (dist < minDist) {
				closest = player;
				minDist = dist;
			}
		}
	}

	return closest;
}

