#pragma once
#include "Observer.h"

namespace dae
{
    class SteamAchievements;

    class SteamAchievementObserver final : public Observer
    {
    public:
        explicit SteamAchievementObserver(SteamAchievements& achievements);

        void Notify(Event event, GameObject* gameObject) override;

    private:
        SteamAchievements& m_SteamAchievements;
    };
}