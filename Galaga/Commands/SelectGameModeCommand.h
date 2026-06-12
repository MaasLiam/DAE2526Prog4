#pragma once

#include "Command.h"
#include "GameMode.h"

class GalagaGameControllerComponent;

class SelectGameModeCommand final : public dae::Command
{
public:
	SelectGameModeCommand(GalagaGameControllerComponent& gameController, galaga::GameMode gameMode);

	void Execute(float deltaTime) override;

private:
	GalagaGameControllerComponent& m_GameController;
	galaga::GameMode m_GameMode{};
};