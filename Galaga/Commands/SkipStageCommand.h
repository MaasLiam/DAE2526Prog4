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

		~SkipStageCommand() override = default;

		SkipStageCommand(const SkipStageCommand&) = delete;
		SkipStageCommand(SkipStageCommand&&) = delete;
		SkipStageCommand& operator=(const SkipStageCommand&) = delete;
		SkipStageCommand& operator=(SkipStageCommand&&) = delete;

	private:
		GalagaGameControllerComponent& m_GameController;
	};
}