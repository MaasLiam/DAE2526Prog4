#include "BulletEnemyCollisionComponent.h"

#include "Scene.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "ScoreComponent.h"
#include "GalagaGameControllerComponent.h"

BulletEnemyCollisionComponent::BulletEnemyCollisionComponent(dae::GameObject* owner, dae::Scene& scene, dae::ScoreComponent& scoreComponent, GalagaGameControllerComponent* gameController)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_ScoreComponent(scoreComponent)
	, m_GameController(gameController)
{}

void BulletEnemyCollisionComponent::Update(float)
{
	auto* bulletCollision = GetOwner()->GetComponent<CollisionComponent>();
	if (!bulletCollision)
		return;

	for (const auto& object : m_Scene.GetObjects())
	{
		auto* enemy = object->GetComponent<EnemyComponent>();
		auto* enemyCollision = object->GetComponent<CollisionComponent>();

		if (!enemy || !enemyCollision || enemy->IsDead())
			continue;

		if (bulletCollision->Overlaps(*enemyCollision))
		{
			if (m_GameController)
			{
				m_GameController->RegisterHit();
			}

			const int scoreValue = enemy->GetScoreValue();

			enemy->TakeDamage();

			if (enemy->IsDead())
			{
				m_ScoreComponent.AddScore(scoreValue);
				m_Scene.Remove(*object);
			}

			m_Scene.Remove(*GetOwner());
			return;
		}
	}
}