#pragma once

namespace DIGGER {
	constexpr float SCREEN_WIDTH{ 640 };
	constexpr float SCREEN_HEIGHT{ 480 };

	constexpr float GAME_WIDTH{ 640 };
	constexpr float GAME_HEIGHT{ 440 };

	constexpr int TILE_SIZE{ 20 };
	constexpr int TILE_GRID_SIZE{ 40 };
	constexpr int DIG_TUNNEL_SIZE{ 18 };

	constexpr int TUNNEL_SAMPLES{ 20 };
	constexpr int RESPAWN_TIME{ 5 };

	constexpr int DIGGER_LIVES{ 4 };
	constexpr int MAX_LEVELS{ 3 };
	constexpr int MAX_LETTERS{ 3 };

	constexpr int MAX_HIGHSCORE_ENTRIES{ 8 };
	constexpr float SCORE_DISTANCE{ 30.f };
};