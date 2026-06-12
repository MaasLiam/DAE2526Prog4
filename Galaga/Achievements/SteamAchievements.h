#pragma once

#ifdef USE_STEAMWORKS
#pragma warning(push)
#pragma warning(disable:4996)
#include <steam_api.h>
#pragma warning(pop)
#endif

namespace dae
{
	class SteamAchievements final
	{
	public:
		SteamAchievements();

		bool Initialize();
		void SetAchievement(const char* id);

		~SteamAchievements() = default;

		SteamAchievements(const SteamAchievements&) = delete;
		SteamAchievements(SteamAchievements&&) = delete;
		SteamAchievements& operator=(const SteamAchievements&) = delete;
		SteamAchievements& operator=(SteamAchievements&&) = delete;

#ifdef USE_STEAMWORKS
		STEAM_CALLBACK(SteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
		STEAM_CALLBACK(SteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
#endif

	private:
		bool m_Initialized{ false };
	};
}