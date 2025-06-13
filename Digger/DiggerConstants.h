#pragma once

namespace DIGGER {

	// Screen and Game Dimensions
	constexpr static float SCREEN_WIDTH{ 640 };
	constexpr static float SCREEN_HEIGHT{ 480 };
	constexpr static float GAME_WIDTH{ 640 };
	constexpr static float GAME_HEIGHT{ 440 };
	constexpr static float WORLD_BORDER_THICKNESS{ 5 };

	// Grid and Tile Settings
	constexpr static int TILE_SIZE{ 20 };
	constexpr static int TILE_GRID_SIZE{ 40 };
	constexpr static int DIG_TUNNEL_SIZE{ 18 };
	constexpr static int TUNNEL_SAMPLES{ 20 };

	// Player and Game Rules
	constexpr static float DIGGER_SPEED{ 50 };
	constexpr static int DIGGER_LIVES{ 4 };
	constexpr static int MAX_LEVELS{ 3 };
	constexpr static int MAX_LETTERS{ 3 };
	constexpr static int RESPAWN_TIME{ 5 };

	// Timing and Transitions
	constexpr static float TRANSITION_TIME{ 5.f };
	constexpr static float DIGGER_ANIMATION_LENGTH{ 0.2f };
	constexpr static float NOBBIN_ANIMATION_LENGTH{ 0.5f };

	// Scoring and UI
	constexpr static int MAX_HIGHSCORE_ENTRIES{ 8 };
	constexpr static float SCORE_DISTANCE{ 30.f };

	// Audio
	constexpr static float SFX_VOLUME{ 0.25f };

	// Physics and World Events
	constexpr static float BAG_DESTROY_HEIGHT{ 60 };

	// Enemies
	constexpr static float NOBBIN_SPEED{ 70 };
}
