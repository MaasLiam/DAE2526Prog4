#include "BulletEnemyCollisionComponent.h"

#include "Scene.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "ScoreComponent.h"
#include "GalagaGameControllerComponent.h"
#include "VersusBossComponent.h"
#include "GameMode.h"
#include "HealthComponent.h"
#include "ServiceLocator.h"
#include "SoundIds.h"

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
	{
		return;
	}

	for (const auto& object : m_Scene.GetObjects())
	{
		auto* versusBoss = object->GetComponent<VersusBossComponent>();
		auto* versusBossCollision = object->GetComponent<CollisionComponent>();
		auto* versusBossHealth = object->GetComponent<dae::HealthComponent>();

		if (versusBoss && versusBossCollision && versusBossHealth)
		{
			if (m_GameController && m_GameController->GetGameMode() == GameMode::Versus && bulletCollision->Overlaps(*versusBossCollision))
			{
				m_GameController->RegisterHit();

				versusBoss->TakeHit();

				m_Scene.Remove(*GetOwner());

				if (versusBoss->IsDead())
				{
					dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::BossGalagaDestroyed), 1.0f);
					m_GameController->SetVersusWinner(1);
				}
				else
				{
					dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::BossGalagaInjured), 1.0f);
				}

				return;
			}
		}

		auto* enemy = object->GetComponent<EnemyComponent>();
		auto* enemyCollision = object->GetComponent<CollisionComponent>();

		if (!enemy || !enemyCollision || enemy->IsDead())
		{
			continue;
		}

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
				if (enemy->GetType() == EnemyType::BossGalaga)
				{
					dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::BossGalagaDestroyed), 1.0f);
				}
				else
				{
					dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::EnemyDestroyed), 1.0f);
				}

				m_ScoreComponent.AddScore(scoreValue);
				m_Scene.Remove(*object);
			}
			else if (enemy->GetType() == EnemyType::BossGalaga)
			{
				dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::BossGalagaInjured), 1.0f);
			}

			m_Scene.Remove(*GetOwner());
			return;
		}
	}
}