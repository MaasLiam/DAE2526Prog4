#pragma once

#include "Command.h"

namespace galaga
{
	class VersusBossComponent;

	class VersusBossDiveCommand final : public dae::Command
	{
	public:
		explicit VersusBossDiveCommand(VersusBossComponent& boss);

		void Execute(float deltaTime) override;

		~VersusBossDiveCommand() override = default;

		VersusBossDiveCommand(const VersusBossDiveCommand&) = delete;
		VersusBossDiveCommand(VersusBossDiveCommand&&) = delete;
		VersusBossDiveCommand& operator=(const VersusBossDiveCommand&) = delete;
		VersusBossDiveCommand& operator=(VersusBossDiveCommand&&) = delete;

	private:
		VersusBossComponent& m_Boss;
	};
}