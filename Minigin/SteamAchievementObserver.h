#pragma once
#include "Observer.h"

namespace dae
{
	class ScoreComponent;
	class SteamAchievements;

	class SteamAchievementObserver final : public Observer
	{
	public:
		SteamAchievementObserver(ScoreComponent& score, SteamAchievements& steamAchievements);
		~SteamAchievementObserver() override;

		void Notify(Event event, GameObject* source) override;

	private:
		ScoreComponent* m_TargetScore{};
		SteamAchievements* m_SteamAchievements{};
		bool m_Unlocked{ false };
	};
}