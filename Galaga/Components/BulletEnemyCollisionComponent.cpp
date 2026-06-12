#include "BulletEnemyCollisionComponent.h"

#include "CapturedFighterComponent.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "GalagaGameControllerComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "ScoreComponent.h"
#include "ServiceLocator.h"
#include "SoundIds.h"
#include "VersusBossComponent.h"

galaga::BulletEnemyCollisionComponent::BulletEnemyCollisionComponent(dae::GameObject* owner, dae::Scene& scene, ScoreComponent& scoreComponent, GalagaGameControllerComponent* gameController)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_ScoreComponent(scoreComponent)
	, m_GameController(gameController)
	, m_BulletCollision(owner->GetComponent<CollisionComponent>())
{}

void galaga::BulletEnemyCollisionComponent::Update(float)
{
	if (m_BulletCollision == nullptr)
	{
		return;
	}

	for (const auto& object : m_Scene.GetObjects())
	{
		if (object == nullptr)
		{
			continue;
		}

		if (TryHandleVersusBossHit(*object))
		{
			return;
		}

		if (TryHandleCapturedFighterHit(*object))
		{
			return;
		}

		if (TryHandleEnemyHit(*object))
		{
			return;
		}
	}
}

bool galaga::BulletEnemyCollisionComponent::TryHandleVersusBossHit(dae::GameObject& object)
{
	if (m_GameController == nullptr || m_GameController->GetGameMode() != galaga::GameMode::Versus)
	{
		return false;
	}

	auto* versusBoss = object.GetComponent<VersusBossComponent>();
	auto* versusBossCollision = object.GetComponent<CollisionComponent>();
	auto* versusBossHealth = object.GetComponent<HealthComponent>();

	if (versusBoss == nullptr || versusBossCollision == nullptr || versusBossHealth == nullptr)
	{
		return false;
	}

	if (!m_BulletCollision->Overlaps(*versusBossCollision))
	{
		return false;
	}

	RegisterHit();
	versusBoss->TakeHit();
	RemoveBullet();

	if (versusBoss->IsDead())
	{
		dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::BossGalagaDestroyed), 1.0f);
		m_GameController->SetVersusWinner(1);
		return true;
	}

	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::BossGalagaInjured), 1.0f);

	return true;
}

bool galaga::BulletEnemyCollisionComponent::TryHandleCapturedFighterHit(dae::GameObject& object)
{
	auto* capturedFighter = object.GetComponent<CapturedFighterComponent>();
	auto* capturedFighterCollision = object.GetComponent<CollisionComponent>();

	if (capturedFighter == nullptr || capturedFighterCollision == nullptr)
	{
		return false;
	}

	if (!m_BulletCollision->Overlaps(*capturedFighterCollision))
	{
		return false;
	}

	RegisterHit();

	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::EnemyDestroyed), 1.0f);

	m_Scene.Remove(object);
	RemoveBullet();

	return true;
}

bool galaga::BulletEnemyCollisionComponent::TryHandleEnemyHit(dae::GameObject& object)
{
	auto* enemy = object.GetComponent<EnemyComponent>();
	auto* enemyCollision = object.GetComponent<CollisionComponent>();

	if (enemy == nullptr || enemyCollision == nullptr || enemy->IsDead())
	{
		return false;
	}

	if (!m_BulletCollision->Overlaps(*enemyCollision))
	{
		return false;
	}

	RegisterHit();

	const int scoreValue = enemy->GetScoreValue();
	enemy->TakeDamage();

	if (enemy->IsDead())
	{
		if (enemy->GetType() == galaga::EnemyType::BossGalaga)
		{
			dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::BossGalagaDestroyed), 1.0f);
		}
		else
		{
			dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::EnemyDestroyed), 1.0f);
		}

		m_ScoreComponent.AddScore(scoreValue);
		m_Scene.Remove(object);
	}
	else if (enemy->GetType() == galaga::EnemyType::BossGalaga)
	{
		dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::BossGalagaInjured), 1.0f);
	}

	RemoveBullet();

	return true;
}

void galaga::BulletEnemyCollisionComponent::RegisterHit() const
{
	if (m_GameController == nullptr)
	{
		return;
	}

	m_GameController->RegisterHit();
}

void galaga::BulletEnemyCollisionComponent::RemoveBullet()
{
	m_Scene.Remove(*GetOwner());
}