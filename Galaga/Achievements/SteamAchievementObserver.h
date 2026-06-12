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

        ~SteamAchievementObserver() override = default;
        SteamAchievementObserver(const SteamAchievementObserver&) = delete;
        SteamAchievementObserver(SteamAchievementObserver&&) = delete;
        SteamAchievementObserver& operator=(const SteamAchievementObserver&) = delete;
        SteamAchievementObserver& operator=(SteamAchievementObserver&&) = delete;

    private:
        SteamAchievements& m_SteamAchievements;
    };
}