#include "EnemySpawnerComponent.h"
#include "Timer.h"
#include "Level.h"
#include "PrototypeSpawner.h"
#include "Nobbin.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "NobbinAIComponent.h"
#include "DiggerMobile.h"

EnemySpawnerComponent::EnemySpawnerComponent(Rinigin::GameObject* gameObject, Level* level) :
	Component(gameObject),
	m_Level(level),

	m_SpawnClock(300000),
	m_MaxEnemies(3),
	m_SpawnRate(20.f)
{
}

void EnemySpawnerComponent::Update()
{
	const bool canSpawn{
		(m_SpawnClock > m_SpawnRate)
	};

	if (canSpawn) {
		// Spawn enemy
		SpawnEnemy();
		m_SpawnClock = 0;
	}
	m_SpawnClock += Rinigin::Timer::GetInstance().deltaTime;
}

void EnemySpawnerComponent::SpawnEnemy()
{
	if (m_Level->GetEnemySpawner().GetInstances().size() >= m_MaxEnemies) return;
	Nobbin* nobbin = m_Level->GetEnemySpawner().Spawn("Nobbin");

	const glm::vec3& bounds{ nobbin->GetCollider()->GetHalfExtents() };
	const glm::vec2 offset{ bounds.x,bounds.y };
	const glm::vec2& spawnPosition{ m_Level->GetEnemySpawnLocation() - offset };

	nobbin->GetCharacterObject()->SetPosition(spawnPosition);
	nobbin->GetCharacterObject()->SetActive(true);
	nobbin->GetCharacterObject()->AddComponent<NobbinAIComponent>(nobbin);
}
