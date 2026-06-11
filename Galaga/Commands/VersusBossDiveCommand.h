#pragma once

#include "Command.h"

class VersusBossComponent;

class VersusBossDiveCommand final : public dae::Command
{
public:
	explicit VersusBossDiveCommand(VersusBossComponent& boss);

	void Execute(float deltaTime) override;

private:
	VersusBossComponent& m_Boss;
};