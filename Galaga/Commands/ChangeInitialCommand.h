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

		~ChangeInitialCommand() override = default;

		ChangeInitialCommand(const ChangeInitialCommand&) = delete;
		ChangeInitialCommand(ChangeInitialCommand&&) = delete;
		ChangeInitialCommand& operator=(const ChangeInitialCommand&) = delete;
		ChangeInitialCommand& operator=(ChangeInitialCommand&&) = delete;

	private:
		GalagaGameControllerComponent& m_GameController;
		int m_Direction{};
	};
}