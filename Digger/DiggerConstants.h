#pragma once

namespace DIGGER {
	constexpr static float SCREEN_WIDTH{ 640 };
	constexpr static float SCREEN_HEIGHT{ 480 };

	constexpr static float GAME_WIDTH{ 640 };
	constexpr static float GAME_HEIGHT{ 440 };

	constexpr static int TILE_SIZE{ 20 };
	constexpr static int TILE_GRID_SIZE{ 40 };
	constexpr static int DIG_TUNNEL_SIZE{ 18 };

	constexpr static int TUNNEL_SAMPLES{ 20 };
	constexpr static int RESPAWN_TIME{ 5 };

	constexpr static int DIGGER_LIVES{ 4 };
	constexpr static int MAX_LEVELS{ 3 };
	constexpr static int MAX_LETTERS{ 3 };

	constexpr static float TRANSITION_TIME{ 5.f };

	constexpr static int MAX_HIGHSCORE_ENTRIES{ 8 };
	constexpr static float SCORE_DISTANCE{ 30.f };

	constexpr static float SFX_VOLUME{ .25f };
};