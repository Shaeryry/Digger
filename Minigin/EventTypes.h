#pragma once

namespace dae {
	enum class EventType {
		// Score events
		ScoreChanged,
		// Player events
		Died,
		HealthChanged,
		// Enemy events
		EnemyDied
	};
}
