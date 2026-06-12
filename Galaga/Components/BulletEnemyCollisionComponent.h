#pragma once

#include "Component.h"

namespace dae
{
	class GameObject;
	class Scene;
}

namespace galaga
{
	class CapturedFighterComponent;
	class CollisionComponent;
	class EnemyComponent;
	class GalagaGameControllerComponent;
	class ScoreComponent;
	class VersusBossComponent;

	class BulletEnemyCollisionComponent final : public dae::Component
	{
	public:
		BulletEnemyCollisionComponent(dae::GameObject* owner, dae::Scene& scene, ScoreComponent& scoreComponent, GalagaGameControllerComponent* gameController = nullptr);

		~BulletEnemyCollisionComponent() override = default;

		BulletEnemyCollisionComponent(const BulletEnemyCollisionComponent&) = delete;
		BulletEnemyCollisionComponent(BulletEnemyCollisionComponent&&) = delete;
		BulletEnemyCollisionComponent& operator=(const BulletEnemyCollisionComponent&) = delete;
		BulletEnemyCollisionComponent& operator=(BulletEnemyCollisionComponent&&) = delete;

		void Update(float deltaTime) override;

	private:
		[[nodiscard]] bool TryHandleVersusBossHit(dae::GameObject& object);
		[[nodiscard]] bool TryHandleCapturedFighterHit(dae::GameObject& object);
		[[nodiscard]] bool TryHandleEnemyHit(dae::GameObject& object);

		void RegisterHit() const;
		void RemoveBullet();

		dae::Scene& m_Scene;
		ScoreComponent& m_ScoreComponent;
		GalagaGameControllerComponent* m_GameController{};
		CollisionComponent* m_BulletCollision{};
	};
}