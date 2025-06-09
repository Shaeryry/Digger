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


NobbinAIComponent::NobbinAIComponent(Rinigin::GameObject* gameObject, Nobbin* nobbin, Level* level) :
	Component(gameObject),
	m_Level(level),
	m_Pathfinder(std::make_unique<Pathfinder>()),
	m_Nobbin(nobbin),
	m_CurrentPathIndex(0),
	m_PathCooldownTimer(0.0f),
	m_PathCooldownDuration(1.0f) // 1 second cooldown
{
	//m_Nobbin->GetRigidbody()->SetCanCollide(false);
	//m_Nobbin->GetCollider()->SetEnabled(false);
}

void NobbinAIComponent::Update()
{
	if (m_CurrentPathIndex >= m_Path.size()) UpdatePath();
	MoveAlongPath();

	m_PathCooldownTimer += Rinigin::Timer::GetInstance().deltaTime;

}

void NobbinAIComponent::Render() const
{
	auto* renderer{ Rinigin::Renderer::GetInstance().GetSDLRenderer() };

	for (auto& point : m_Path) {
		const SDL_Rect rect{
			static_cast<int>(point.x),
			static_cast<int>(point.y),
			DIGGER::TILE_GRID_SIZE,
			DIGGER::TILE_GRID_SIZE
		};

		if (m_CurrentPathIndex < m_Path.size()){
			if (point == m_Path[m_CurrentPathIndex]) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // rojo
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue
			}

			SDL_RenderDrawRect(renderer, &rect);
		}
	}
}

Character* NobbinAIComponent::FindClosestPlayer() const
{
	Character* closest = nullptr;
	float minDist = std::numeric_limits<float>::max();
	glm::vec3 myPos = GetOwner()->GetWorldPosition();

	for (int i = 0; i < m_Level->GetPlayers().size(); i++) {
		Character* player = m_Level->GetPlayer(i);
		if (!player || !player->GetCharacterObject()->IsActive()) continue;

		glm::vec3 playerPos = player->GetCharacterObject()->GetWorldPosition();
		float dist = glm::distance(myPos, playerPos);

		if (dist < minDist) {
			closest = player;
			minDist = dist;
		}
	}

	return closest;
}

void NobbinAIComponent::MoveAlongPath()
{
	if (!m_Path.empty() && m_CurrentPathIndex < m_Path.size())
	{
		const glm::vec2 currentWayPoint = m_Path[m_CurrentPathIndex];
		const glm::vec2 offsetWayPoint = currentWayPoint + glm::vec2(DIGGER::TILE_GRID_SIZE, DIGGER::TILE_GRID_SIZE)/2.f;

		glm::vec3 currentPos = GetOwner()->GetWorldPosition() + m_Nobbin->GetCollider()->GetHalfExtents(); 
		glm::vec3 targetPos = glm::vec3(offsetWayPoint.x, offsetWayPoint.y, 0); // flatten to same Z

		glm::vec3 direction = glm::normalize(targetPos - currentPos);
		float distance = glm::distance(currentPos, targetPos);

		// If close enough to target node, move to next one
		if (distance < 2.0f) // within 2 units
		{
			m_CurrentPathIndex++;
			//UpdatePath();
		}
		else
		{
			// Apply movement
			std::cout << direction.x << ":" << direction.y << std::endl;
			m_Nobbin->GetMoveCommand()->SetDirection(direction);
			m_Nobbin->GetMoveCommand()->Execute();
		}
	}
}

void NobbinAIComponent::UpdatePath()
{
	Character* closestPlayer = FindClosestPlayer();
	if (closestPlayer) {
		glm::vec3 fromPosition = GetOwner()->GetWorldPosition() + m_Nobbin->GetCollider()->GetHalfExtents();
		glm::vec3 targetPosition = closestPlayer->GetCharacterObject()->GetWorldPosition() + closestPlayer->GetCollider()->GetHalfExtents();
		auto foundPath = m_Pathfinder->FindPath(m_Level->Map(), fromPosition, targetPosition, glm::vec2(DIGGER::TILE_GRID_SIZE,DIGGER::TILE_GRID_SIZE));

		if (not foundPath.empty()) {
			m_Path = foundPath;
			m_CurrentPathIndex = 0;
			std::cout << "RESET" << std::endl;
		};

		m_PathCooldownTimer = 0;
	};
}
