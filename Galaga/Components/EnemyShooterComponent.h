#pragma once

#include "Component.h"

namespace dae
{
	class Scene;
}

namespace galaga
{
	class GalagaGameControllerComponent;
	class EnemyShooterComponent final : public dae::Component
	{
	public:
		EnemyShooterComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController);

		void Update(float deltaTime) override;

	private:
		void Shoot();

		dae::Scene& m_Scene;
		GalagaGameControllerComponent& m_GameController;

		float m_ShootTimer{};
		bool m_HasShotDuringCurrentDive{};
	};
}