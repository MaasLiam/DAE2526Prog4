#pragma once

#include "Game.h"
#include "SteamAchievements.h"
#include "SteamAchievementObserver.h"

#include <memory>

class Galaga final : public Game
{
public:
	Galaga() = default;
	~Galaga() override = default;

	Galaga(const Galaga&) = delete;
	Galaga(Galaga&&) = delete;
	Galaga& operator=(const Galaga&) = delete;
	Galaga& operator=(Galaga&&) = delete;

	void Initialize() override;

private:
	std::unique_ptr<dae::SteamAchievements> m_pSteamAchievements{};
	std::unique_ptr<dae::SteamAchievementObserver> m_pSteamAchievementObserver{};
};