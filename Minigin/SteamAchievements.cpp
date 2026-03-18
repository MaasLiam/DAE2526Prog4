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

	bool SteamAchievements::SetAchievement(const char* id)
	{
#ifdef USE_STEAMWORKS
		if (!m_Initialized)
		{
			return false;
		}

		SteamUserStats()->SetAchievement(id);
		return SteamUserStats()->StoreStats();
#else
		(void)id;
		return false;
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