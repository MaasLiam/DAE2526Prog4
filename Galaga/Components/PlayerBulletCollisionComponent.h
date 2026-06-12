#pragma once

#include "Component.h"

namespace dae
{
	class GameObject;
	class Scene;
}

namespace galaga
{
	class CollisionComponent;
	class HealthComponent;

	class PlayerBulletCollisionComponent final : public dae::Component
	{
	public:
		PlayerBulletCollisionComponent(dae::GameObject* owner, dae::Scene& scene, dae::GameObject& player);
		~PlayerBulletCollisionComponent() override = default;

		PlayerBulletCollisionComponent(const PlayerBulletCollisionComponent&) = delete;
		PlayerBulletCollisionComponent(PlayerBulletCollisionComponent&&) = delete;
		PlayerBulletCollisionComponent& operator=(const PlayerBulletCollisionComponent&) = delete;
		PlayerBulletCollisionComponent& operator=(PlayerBulletCollisionComponent&&) = delete;

		void Update(float deltaTime) override;

	private:
		dae::Scene& m_Scene;
		dae::GameObject& m_Player;

		CollisionComponent* m_BulletCollision{};
		CollisionComponent* m_PlayerCollision{};
		HealthComponent* m_PlayerHealth{};
	};
}