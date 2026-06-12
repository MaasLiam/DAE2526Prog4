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
	class CollisionComponent;

	class EnemyBulletCollisionComponent final : public dae::Component
	{
	public:
		EnemyBulletCollisionComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController);
		~EnemyBulletCollisionComponent() override = default;

		EnemyBulletCollisionComponent(const EnemyBulletCollisionComponent&) = delete;
		EnemyBulletCollisionComponent(EnemyBulletCollisionComponent&&) = delete;
		EnemyBulletCollisionComponent& operator=(const EnemyBulletCollisionComponent&) = delete;
		EnemyBulletCollisionComponent& operator=(EnemyBulletCollisionComponent&&) = delete;

		void Update(float deltaTime) override;

	private:
		bool TryHitPlayer(galaga::PlayerIndex playerIndex);

		dae::Scene& m_Scene;
		GalagaGameControllerComponent& m_GameController;
		CollisionComponent* m_BulletCollision{};
	};
}