#pragma once
#include <memory>
#include "Singleton.h"
#include "Observer.h"
#include "CSteamAchievements.h"
#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }

namespace dae {
	class GameObject;
	class Achievement final : public Singleton<Achievement>,public Observer
	{
	public:
		Achievement();
		virtual void Notify(EventType event,GameObject* gameObject) override;
	private:
		// Achievement 
		SteamAchievement::AchievementData* GetAchievementById(const SteamAchievement::AchievementID id);
		void UnlockAchievement(SteamAchievement::AchievementData* achievement);

		int m_AmountAchievements;
		SteamAchievement::AchievementData m_Achievements[1]{
			{SteamAchievement::AchievementID::ACH_WIN_ONE_GAME, "ACH_WIN_ONE_GAME"}
		};

		// Steam
		std::unique_ptr<SteamAchievement::CSteamAchievements> m_SteamAchievements;
	};
}


