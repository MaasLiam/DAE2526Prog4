#include "SteamAchievements.h"

namespace dae
{
	SteamAchievements::SteamAchievements()
#ifdef USE_STEAMWORKS
		: m_CallbackUserStatsStored(this, &SteamAchievements::OnUserStatsStored)
		, m_CallbackAchievementStored(this, &SteamAchievements::OnAchievementStored)
#endif
	{
		m_Initialized = Initialize();
	}

	bool SteamAchievements::Initialize()
	{
#ifdef USE_STEAMWORKS
		if (SteamUserStats() == nullptr || SteamUser() == nullptr)
		{
			return false;
		}

		if (!SteamUser()->BLoggedOn())
		{
			return false;
		}

		return true;
#else
		return false;
#endif
	}

	void SteamAchievements::SetAchievement(const char* id)
	{
#ifdef USE_STEAMWORKS
		bool achieved{ false };

		if (SteamUserStats()->GetAchievement(id, &achieved) && achieved)
			return;

		SteamUserStats()->SetAchievement(id);
		SteamUserStats()->StoreStats();
#else
		(void)id;
		return;
#endif
	}

#ifdef USE_STEAMWORKS
	void SteamAchievements::OnUserStatsStored(UserStatsStored_t*)
	{
	}

	void SteamAchievements::OnAchievementStored(UserAchievementStored_t*)
	{
	}
#endif
}