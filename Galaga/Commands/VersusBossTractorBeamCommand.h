#pragma once

#include "Command.h"

namespace galaga
{
	class VersusBossComponent;

	class VersusBossTractorBeamCommand final : public dae::Command
	{
	public:
		explicit VersusBossTractorBeamCommand(VersusBossComponent& boss);

		void Execute(float deltaTime) override;

		~VersusBossTractorBeamCommand() override = default;

		VersusBossTractorBeamCommand(const VersusBossTractorBeamCommand&) = delete;
		VersusBossTractorBeamCommand(VersusBossTractorBeamCommand&&) = delete;
		VersusBossTractorBeamCommand& operator=(const VersusBossTractorBeamCommand&) = delete;
		VersusBossTractorBeamCommand& operator=(VersusBossTractorBeamCommand&&) = delete;

	private:
		VersusBossComponent& m_Boss;
	};
}