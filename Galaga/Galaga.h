#pragma once
#include "Game.h"
#include "SteamAchievements.h"
#include "SteamAchievementObserver.h"
#include <memory>

class Galaga final : public Game
{
public:
	void Initialize() override;
private:
	std::unique_ptr<dae::SteamAchievements> m_pSteamAchievements{};
	std::unique_ptr<dae::SteamAchievementObserver> m_pSteamAchievementObserver{};
};