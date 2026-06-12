#pragma once

#include "Command.h"
#include "GameObject.h"
#include "ControlContext.h"

namespace dae
{
	class Scene;
}

namespace galaga
{
	class GalagaGameControllerComponent;
	enum class ShootOwner
	{
		PlayerOne,
		PlayerTwo
	};

	class ShootCommand final : public dae::Command
	{
	public:
		ShootCommand(dae::GameObject& shooter, dae::Scene& scene, GalagaGameControllerComponent* gameController = nullptr, ShootOwner owner = ShootOwner::PlayerOne, ControlContext controlContext = ControlContext::Always);
		void Execute(float deltaTime) override;

		~ShootCommand() override = default;

		ShootCommand(const ShootCommand&) = delete;
		ShootCommand(ShootCommand&&) = delete;
		ShootCommand& operator=(const ShootCommand&) = delete;
		ShootCommand& operator=(ShootCommand&&) = delete;

	private:
		dae::GameObject& m_Shooter;
		dae::Scene& m_Scene;
		GalagaGameControllerComponent* m_GameController{};
		ShootOwner m_Owner{ ShootOwner::PlayerOne };
		ControlContext m_ControlContext{ ControlContext::Always };
	};
}