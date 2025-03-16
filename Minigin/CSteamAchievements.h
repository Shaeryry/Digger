#pragma once
#include <steam_api.h>

namespace SteamAchievement {
	enum class AchievementID
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_HEAVY_FIRE = 2,
		ACH_TRAVEL_FAR_ACCUM = 3,
		ACH_TRAVEL_FAR_SINGLE = 4,
	};

	struct AchievementData
	{
		AchievementID m_eAchievementID;
		const char* m_pchAchievementID;
		char m_rgchName[128];
		char m_rgchDescription[256];
		bool m_bAchieved;
		int m_iIconImage;
	};

	class CSteamAchievements final
	{
	private:
		__int64 m_iAppID; // Our current AppID
		AchievementData* m_pAchievements; // Achievements data
		int m_iNumAchievements; // The number of Achievements
		bool m_bInitialized; // Have we called Request stats and received the callback?
	public:
		CSteamAchievements(AchievementData* Achievements, int NumAchievements);
		~CSteamAchievements() = default;

		bool RequestStats();
		bool SetAchievement(const char* ID);
		STEAM_CALLBACK(CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
		STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
		STEAM_CALLBACK(CSteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
	};
}


