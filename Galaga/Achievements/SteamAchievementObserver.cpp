#include "SteamAchievementObserver.h"
#include "SteamAchievements.h"
#include "ScoreComponent.h"
#include "GameObject.h"
#include "Event.h"  

using namespace dae;

SteamAchievementObserver::SteamAchievementObserver(SteamAchievements& achievements)
    : m_SteamAchievements{ achievements }
{
}

void SteamAchievementObserver::Notify(Event event, GameObject* gameObject)
{
    if (event != Event::ScoreChanged)
        return;

    if (!gameObject)
        return;

    auto* scoreComponent = gameObject->GetComponent<galaga::ScoreComponent>();
    if (!scoreComponent)
        return;

    if (scoreComponent->GetScore() >= 500)
    {
        m_SteamAchievements.SetAchievement("ACH_WIN_ONE_GAME");
    }
}