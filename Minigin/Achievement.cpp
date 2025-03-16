#include "Achievement.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include <iostream>


dae::Achievement::Achievement() :
	m_AmountAchievements{ sizeof(m_Achievements) / sizeof(SteamAchievement::AchievementData) },
	m_SteamAchievements{ std::make_unique<SteamAchievement::CSteamAchievements>(m_Achievements,m_AmountAchievements) }
{
}

void dae::Achievement::Notify(EventType event, GameObject* gameObject)
{
	switch (event)
	{
	case dae::EventType::ScoreChanged:
		auto scoreComponent = gameObject->GetComponent<dae::ScoreComponent>();
		if (scoreComponent) {
			if (scoreComponent->GetScore() >= 500) {
				UnlockAchievement(GetAchievementById(SteamAchievement::AchievementID::ACH_WIN_ONE_GAME));
			}
		}

		break;
	}
}

SteamAchievement::AchievementData* dae::Achievement::GetAchievementById(const SteamAchievement::AchievementID id)
{

	for (int achievementIndex{ 0 }; achievementIndex < m_AmountAchievements; achievementIndex++) {
		auto& currentAchievement{ m_Achievements[achievementIndex] };
		if (currentAchievement.m_eAchievementID == id) {
			return &currentAchievement;
		}
	}

	return nullptr;
}

void dae::Achievement::UnlockAchievement(SteamAchievement::AchievementData* achievement)
{
	if (achievement->m_bAchieved) return;
	achievement->m_bAchieved = true;
	achievement->m_iIconImage = 0;

	// mark it down
	m_SteamAchievements->SetAchievement(achievement->m_pchAchievementID);

}
