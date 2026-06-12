#pragma once

#include "Component.h"

namespace dae
{
	class Scene;
}

namespace galaga
{
	class ScoreComponent;
	class GalagaGameControllerComponent;
	class BulletEnemyCollisionComponent final : public dae::Component
	{
	public:
		BulletEnemyCollisionComponent(dae::GameObject* owner, dae::Scene& scene, ScoreComponent& scoreComponent, GalagaGameControllerComponent* gameController = nullptr);
		void Update(float deltaTime) override;

		~BulletEnemyCollisionComponent() override = default;

		BulletEnemyCollisionComponent(const BulletEnemyCollisionComponent&) = delete;
		BulletEnemyCollisionComponent(BulletEnemyCollisionComponent&&) = delete;
		BulletEnemyCollisionComponent& operator=(const BulletEnemyCollisionComponent&) = delete;
		BulletEnemyCollisionComponent& operator=(BulletEnemyCollisionComponent&&) = delete;

	private:
		dae::Scene& m_Scene;
		ScoreComponent& m_ScoreComponent;
		GalagaGameControllerComponent* m_GameController{};
	};
}