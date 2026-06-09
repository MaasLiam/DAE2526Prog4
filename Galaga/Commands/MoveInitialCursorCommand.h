#pragma once

#include "Command.h"

class GalagaGameControllerComponent;

class MoveInitialCursorCommand final : public dae::Command
{
public:
	MoveInitialCursorCommand(GalagaGameControllerComponent& gameController, int direction);

	void Execute(float deltaTime) override;

private:
	GalagaGameControllerComponent& m_GameController;
	int m_Direction{};
};