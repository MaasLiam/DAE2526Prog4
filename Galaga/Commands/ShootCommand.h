#pragma once

#include "Command.h"
#include "GameObject.h"

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
		ShootCommand(dae::GameObject& shooter, dae::Scene& scene, GalagaGameControllerComponent* gameController = nullptr, ShootOwner owner = ShootOwner::PlayerOne);
		void Execute(float deltaTime) override;

	private:
		dae::GameObject& m_Shooter;
		dae::Scene& m_Scene;
		GalagaGameControllerComponent* m_GameController{};
		ShootOwner m_Owner{ ShootOwner::PlayerOne };
	};
}