#pragma once
#include "Component.h"

class Level;
class EnemySpawnerComponent final : public Rinigin::Component
{
public:
	explicit EnemySpawnerComponent(Rinigin::GameObject* gameObject,Level* level);
	void Update() override;
private:
	Level* m_Level;
	void SpawnEnemy();

	int m_MaxEnemies;
	float m_SpawnRate;
	float m_SpawnClock;
};

