#pragma once

#include "Command.h"
#include "GameMode.h"

namespace galaga
{
	class GalagaGameControllerComponent;
	class SelectGameModeCommand final : public dae::Command
	{
	public:
		SelectGameModeCommand(GalagaGameControllerComponent& gameController, GameMode gameMode);

		void Execute(float deltaTime) override;
		~SelectGameModeCommand() override = default;

		SelectGameModeCommand(const SelectGameModeCommand&) = delete;
		SelectGameModeCommand(SelectGameModeCommand&&) = delete;
		SelectGameModeCommand& operator=(const SelectGameModeCommand&) = delete;
		SelectGameModeCommand& operator=(SelectGameModeCommand&&) = delete;

	private:
		GalagaGameControllerComponent& m_GameController;
		GameMode m_GameMode{};
	};
}