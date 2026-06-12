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

	private:
		dae::Scene& m_Scene;
		ScoreComponent& m_ScoreComponent;
		GalagaGameControllerComponent* m_GameController{};
	};
}