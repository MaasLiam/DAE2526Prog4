#pragma once

#include "Command.h"

class VersusBossComponent;

class VersusBossTractorBeamCommand final : public dae::Command
{
public:
	explicit VersusBossTractorBeamCommand(VersusBossComponent& boss);

	void Execute(float deltaTime) override;

private:
	VersusBossComponent& m_Boss;
};