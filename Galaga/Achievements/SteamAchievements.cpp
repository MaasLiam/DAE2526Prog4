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
		if (SteamUser() == nullptr)
		{
			return false;
		}

		if (SteamUserStats() == nullptr)
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
		if (!m_Initialized)
		{
			return;
		}

		if (id == nullptr)
		{
			return;
		}

		auto* userStats = SteamUserStats();
		if (userStats == nullptr)
		{
			m_Initialized = false;
			return;
		}

		bool achieved{ false };
		if (userStats->GetAchievement(id, &achieved) && achieved)
		{
			return;
		}

		userStats->SetAchievement(id);
		userStats->StoreStats();
#else
		(void)id;
#endif
	}

#ifdef USE_STEAMWORKS
	void SteamAchievements::OnUserStatsStored(UserStatsStored_t*)
	{}

	void SteamAchievements::OnAchievementStored(UserAchievementStored_t*)
	{}
#endif
}