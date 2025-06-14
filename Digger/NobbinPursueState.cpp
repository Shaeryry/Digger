#include "NobbinPursueState.h"
#include "Timer.h"
#include "Renderer.h"
#include "DiggerConstants.h"
#include "Nobbin.h"
#include "GameObject.h"
#include "Pathfinder.h"
#include "Level.h"
#include "MovementCommands.h"

NobbinPursueState::NobbinPursueState(Rinigin::StateContextComponent* context, Nobbin* nobbin) :
	State(context),
	m_Nobbin(nobbin),
	m_CurrentPathIndex(0),
	m_PathCooldownTimer(0.0f),
	m_PathCooldownDuration(1.0f)
{
}

void NobbinPursueState::Enter()
{
}

Rinigin::State* NobbinPursueState::Update()
{

	if (m_PathCooldownTimer > m_PathCooldownDuration) {
		//if (m_CurrentPathIndex >= m_Path.size()) {
			UpdatePath();
			m_PathCooldownTimer = 0;
		//};
	}

	MoveAlongPath();
	m_PathCooldownTimer += Rinigin::Timer::GetInstance().deltaTime;
	return nullptr;
}

void NobbinPursueState::MoveAlongPath() {
	if (!m_Path.empty() && m_CurrentPathIndex < m_Path.size())
	{
		const glm::vec2 currentWayPoint = m_Path[m_CurrentPathIndex];
		const glm::vec2 offsetWayPoint = currentWayPoint + glm::vec2(DIGGER::TILE_GRID_SIZE, DIGGER::TILE_GRID_SIZE) / 2.f;

		glm::vec3 currentPos = m_Nobbin->GetCenteredPosition();//GetOwner()->GetWorldPosition() + m_Nobbin->GetCollider()->GetHalfExtents();
		glm::vec3 targetPos = glm::vec3(offsetWayPoint.x, offsetWayPoint.y, 0); // flatten to same Z

		glm::vec3 direction = glm::normalize(targetPos - currentPos);
		float distance = glm::distance(currentPos, targetPos);

		if (distance < 2.0f) // within 2 units
		{
			m_CurrentPathIndex++;
		}
		else
		{
			m_Nobbin->GetMoveCommand()->SetDirection(direction);
			m_Nobbin->GetMoveCommand()->Execute();
		}
	}
}

void NobbinPursueState::DrawPath() const
{
	auto* renderer{ Rinigin::Renderer::GetInstance().GetSDLRenderer() };

	for (auto& point : m_Path) {
		const SDL_Rect rect{
			static_cast<int>(point.x),
			static_cast<int>(point.y),
			DIGGER::TILE_GRID_SIZE,
			DIGGER::TILE_GRID_SIZE
		};

		if (m_CurrentPathIndex < m_Path.size()) {
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

void NobbinPursueState::UpdatePath()
{
	glm::vec3 fromPosition = m_Nobbin->GetCharacterObject()->GetWorldPosition(); //GetOwner()->GetWorldPosition() + m_Nobbin->GetCollider()->GetHalfExtents();
	glm::vec3 targetPosition = glm::vec3(m_Target, 0); //closestPlayer->GetCharacterObject()->GetWorldPosition() + closestPlayer->GetCollider()->GetHalfExtents();

	auto foundPath = m_Pathfinder->FindPath(m_Nobbin->GetLevel()->Map(), fromPosition, targetPosition, glm::vec2(DIGGER::TILE_GRID_SIZE, DIGGER::TILE_GRID_SIZE));

	if (not foundPath.empty()) {
		m_Path = foundPath;
		m_CurrentPathIndex = 0;
	};

	m_PathCooldownTimer = 0;
}


void NobbinPursueState::Exit()
{
}
