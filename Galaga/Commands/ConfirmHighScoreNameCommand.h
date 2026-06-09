#pragma once

#include "Command.h"

class GalagaGameControllerComponent;

class ConfirmHighScoreNameCommand final : public dae::Command
{
public:
	explicit ConfirmHighScoreNameCommand(GalagaGameControllerComponent& gameController);

	void Execute(float deltaTime) override;

private:
	GalagaGameControllerComponent& m_GameController;
};