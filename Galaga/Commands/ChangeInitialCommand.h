#pragma once

#include "Command.h"


namespace galaga
{
	class GalagaGameControllerComponent;
	class ChangeInitialCommand final : public dae::Command
	{
	public:
		ChangeInitialCommand(GalagaGameControllerComponent& gameController, int direction);

		void Execute(float deltaTime) override;

	private:
		GalagaGameControllerComponent& m_GameController;
		int m_Direction{};
	};
}