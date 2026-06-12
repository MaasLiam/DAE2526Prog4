#pragma once

#include "Command.h"
#include "ControlContext.h"

namespace dae
{
	class GameObject;
	class Scene;
	class TransformComponent;
}

namespace galaga
{
	class GalagaGameControllerComponent;
	class MissileLimitComponent;
	class ScoreComponent;

	enum class ShootOwner
	{
		PlayerOne,
		PlayerTwo
	};

	class ShootCommand final : public dae::Command
	{
	public:
		ShootCommand(
			dae::GameObject& shooter,
			dae::Scene& scene,
			GalagaGameControllerComponent* gameController = nullptr,
			ShootOwner owner = ShootOwner::PlayerOne,
			ControlContext controlContext = ControlContext::Always);

		~ShootCommand() override = default;

		ShootCommand(const ShootCommand&) = delete;
		ShootCommand(ShootCommand&&) = delete;
		ShootCommand& operator=(const ShootCommand&) = delete;
		ShootCommand& operator=(ShootCommand&&) = delete;

		void Execute(float deltaTime) override;

	private:
		bool CanShootForCurrentState() const;
		void SpawnBullet();

		dae::GameObject& m_Shooter;
		dae::Scene& m_Scene;
		GalagaGameControllerComponent* m_GameController{};
		MissileLimitComponent* m_MissileLimit{};
		dae::TransformComponent* m_ShooterTransform{};
		ScoreComponent* m_ScoreComponent{};

		ShootOwner m_Owner{ ShootOwner::PlayerOne };
		ControlContext m_ControlContext{ ControlContext::Always };
	};
}