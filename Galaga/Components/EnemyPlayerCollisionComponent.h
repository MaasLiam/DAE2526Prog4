#pragma once

#include "Component.h"
#include "GalagaGameControllerComponent.h"

#include <vector>

namespace dae
{
	class GameObject;
	class Scene;
}

namespace galaga
{
	class CollisionComponent;
	class EnemyComponent;
	class HealthComponent;

	class EnemyPlayerCollisionComponent final : public dae::Component
	{
	public:
		EnemyPlayerCollisionComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController);
		~EnemyPlayerCollisionComponent() override = default;

		EnemyPlayerCollisionComponent(const EnemyPlayerCollisionComponent&) = delete;
		EnemyPlayerCollisionComponent(EnemyPlayerCollisionComponent&&) = delete;
		EnemyPlayerCollisionComponent& operator=(const EnemyPlayerCollisionComponent&) = delete;
		EnemyPlayerCollisionComponent& operator=(EnemyPlayerCollisionComponent&&) = delete;

		void Update(float deltaTime) override;

		void AddPlayer(dae::GameObject* player);

	private:
		bool TryHandleEnemyCollision(dae::GameObject& object);
		bool TryHandlePlayerCollision(EnemyComponent& enemy, CollisionComponent& enemyCollision, dae::GameObject& player, PlayerIndex playerIndex, dae::GameObject& enemyObject);
		dae::Scene& m_Scene;
		GalagaGameControllerComponent& m_GameController;
		std::vector<dae::GameObject*> m_Players{};
	};
}