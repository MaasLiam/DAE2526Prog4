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
		bool SetAchievement(const char* id);

#ifdef USE_STEAMWORKS
		STEAM_CALLBACK(SteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
		STEAM_CALLBACK(SteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
#endif

	private:
		bool m_Initialized{ false };
	};
}