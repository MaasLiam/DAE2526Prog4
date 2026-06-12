#pragma once

#include "Command.h"

namespace galaga
{
	class GalagaGameControllerComponent;
	class ConfirmHighScoreNameCommand final : public dae::Command
	{
	public:
		explicit ConfirmHighScoreNameCommand(GalagaGameControllerComponent& gameController);

		void Execute(float deltaTime) override;

		~ConfirmHighScoreNameCommand() override = default;

		ConfirmHighScoreNameCommand(const ConfirmHighScoreNameCommand&) = delete;
		ConfirmHighScoreNameCommand(ConfirmHighScoreNameCommand&&) = delete;
		ConfirmHighScoreNameCommand& operator=(const ConfirmHighScoreNameCommand&) = delete;
		ConfirmHighScoreNameCommand& operator=(ConfirmHighScoreNameCommand&&) = delete;

	private:
		GalagaGameControllerComponent& m_GameController;
	};
}