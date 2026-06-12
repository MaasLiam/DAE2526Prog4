#pragma once

#include "Command.h"

namespace galaga
{
	class GalagaGameControllerComponent;
	class SkipStageCommand final : public dae::Command
	{
	public:
		explicit SkipStageCommand(GalagaGameControllerComponent& gameController);

		void Execute(float deltaTime) override;

	private:
		GalagaGameControllerComponent& m_GameController;
	};
}