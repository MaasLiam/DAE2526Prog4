#pragma once

#include "Command.h"

namespace dae
{
	class GameObject;
	class Scene;
}

namespace galaga
{
	class GalagaGameControllerComponent;
	class VersusBossShootCommand final : public dae::Command
	{
	public:
		VersusBossShootCommand(dae::GameObject& boss, dae::GameObject& targetPlayer, dae::Scene& scene, GalagaGameControllerComponent& gameController);

		void Execute(float deltaTime) override;

		~VersusBossShootCommand() override = default;

		VersusBossShootCommand(const VersusBossShootCommand&) = delete;
		VersusBossShootCommand(VersusBossShootCommand&&) = delete;
		VersusBossShootCommand& operator=(const VersusBossShootCommand&) = delete;
		VersusBossShootCommand& operator=(VersusBossShootCommand&&) = delete;

	private:
		dae::GameObject& m_Boss;
		dae::GameObject& m_TargetPlayer;
		dae::Scene& m_Scene;
		GalagaGameControllerComponent& m_GameController;
	};
}