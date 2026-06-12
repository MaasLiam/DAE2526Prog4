#pragma once

#include "Command.h"

namespace galaga
{
	class GalagaGameControllerComponent;
	class MoveInitialCursorCommand final : public dae::Command
	{
	public:
		MoveInitialCursorCommand(GalagaGameControllerComponent& gameController, int direction);

		void Execute(float deltaTime) override;

		~MoveInitialCursorCommand() override = default;

		MoveInitialCursorCommand(const MoveInitialCursorCommand&) = delete;
		MoveInitialCursorCommand(MoveInitialCursorCommand&&) = delete;
		MoveInitialCursorCommand& operator=(const MoveInitialCursorCommand&) = delete;
		MoveInitialCursorCommand& operator=(MoveInitialCursorCommand&&) = delete;

	private:
		GalagaGameControllerComponent& m_GameController;
		int m_Direction{};
	};
}