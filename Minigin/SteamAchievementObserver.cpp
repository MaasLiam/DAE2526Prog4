#include "SteamAchievementObserver.h"
#include "ScoreComponent.h"
#include "SteamAchievements.h"
#include "Event.h"

namespace dae
{
	SteamAchievementObserver::SteamAchievementObserver(ScoreComponent& score, SteamAchievements& steamAchievements)
		: m_TargetScore(&score)
		, m_SteamAchievements(&steamAchievements)
	{
		m_TargetScore->GetSubject().AddObserver(this);
	}

	SteamAchievementObserver::~SteamAchievementObserver()
	{
		if (m_TargetScore)
		{
			m_TargetScore->GetSubject().RemoveObserver(this);
		}
	}

	void SteamAchievementObserver::Notify(Event event, GameObject* /*source*/)
	{
		if (m_Unlocked || event != Event::ScoreChanged)
		{
			return;
		}

		if (m_TargetScore->GetScore() >= 500)
		{
			if (m_SteamAchievements && m_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME"))
			{
				m_Unlocked = true;
			}
			
		}
	}
}