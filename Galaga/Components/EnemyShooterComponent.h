#pragma once

#include "Component.h"

namespace dae
{
	class Scene;
	class TransformComponent;
}

namespace galaga
{
	class EnemyComponent;
	class GalagaGameControllerComponent;

	class EnemyShooterComponent final : public dae::Component
	{
	public:
		EnemyShooterComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController);
		~EnemyShooterComponent() override = default;

		EnemyShooterComponent(const EnemyShooterComponent&) = delete;
		EnemyShooterComponent(EnemyShooterComponent&&) = delete;
		EnemyShooterComponent& operator=(const EnemyShooterComponent&) = delete;
		EnemyShooterComponent& operator=(EnemyShooterComponent&&) = delete;

		void Update(float deltaTime) override;

	private:
		void Shoot();

		dae::Scene& m_Scene;
		GalagaGameControllerComponent& m_GameController;
		EnemyComponent* m_Enemy{};
		dae::TransformComponent* m_Transform{};

		float m_ShootTimer{};
		bool m_HasShotDuringCurrentDive{};
	};
}