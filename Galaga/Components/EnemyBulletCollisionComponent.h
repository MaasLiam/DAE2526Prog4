#pragma once

#include "Component.h"
#include "GalagaGameControllerComponent.h"

namespace dae
{
	class GameObject;
	class Scene;
}

namespace galaga
{
	class EnemyBulletCollisionComponent final : public dae::Component
	{
	public:
		EnemyBulletCollisionComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController);

		void Update(float deltaTime) override;

		~EnemyBulletCollisionComponent() override = default;

		EnemyBulletCollisionComponent(const EnemyBulletCollisionComponent&) = delete;
		EnemyBulletCollisionComponent(EnemyBulletCollisionComponent&&) = delete;
		EnemyBulletCollisionComponent& operator=(const EnemyBulletCollisionComponent&) = delete;
		EnemyBulletCollisionComponent& operator=(EnemyBulletCollisionComponent&&) = delete;

	private:
		bool TryHitPlayer(galaga::PlayerIndex playerIndex);

		dae::Scene& m_Scene;
		GalagaGameControllerComponent& m_GameController;
	};
}