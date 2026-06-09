#pragma once

#include "Component.h"

namespace dae
{
	class Scene;
	class ScoreComponent;
}

class GalagaGameControllerComponent;

class BulletEnemyCollisionComponent final : public dae::Component
{
public:
	BulletEnemyCollisionComponent(dae::GameObject* owner, dae::Scene& scene, dae::ScoreComponent& scoreComponent, GalagaGameControllerComponent* gameController = nullptr);
	void Update(float deltaTime) override;

private:
	dae::Scene& m_Scene;
	dae::ScoreComponent& m_ScoreComponent;
	GalagaGameControllerComponent* m_GameController{};
};