#include "ShootCommand.h"

#include "BulletComponent.h"
#include "BulletEnemyCollisionComponent.h"
#include "CollisionComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "GameState.h"
#include "HealthComponent.h"
#include "MissileLimitComponent.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "ScoreComponent.h"
#include "ServiceLocator.h"
#include "SoundIds.h"
#include "TransformComponent.h"
#include "GalagaGameControllerComponent.h"


ShootCommand::ShootCommand(dae::GameObject& shooter, dae::Scene& scene, GalagaGameControllerComponent* gameController, ShootOwner owner)
	: m_Shooter(shooter)
	, m_Scene(scene)
	, m_GameController(gameController)
	, m_Owner(owner)
{

}

void ShootCommand::Execute(float)
{
	if (m_GameController && m_GameController->GetState() != GameState::Playing)
	{
		return;
	}

	if (m_GameController && m_Owner == ShootOwner::PlayerTwo && m_GameController->GetGameMode() == GameMode::Versus)
	{
		return;
	}

	if (m_GameController)
	{
		const PlayerIndex playerIndex = m_Owner == ShootOwner::PlayerOne ? PlayerIndex::PlayerOne : PlayerIndex::PlayerTwo;

		if (!m_GameController->CanPlayerAct(playerIndex))
		{
			return;
		}
	}

	auto* missileLimit = m_Shooter.GetComponent<MissileLimitComponent>();
	if (!missileLimit || !missileLimit->CanShoot())
	{
		return;
	}

	auto* shooterTransform = m_Shooter.GetComponent<dae::TransformComponent>();
	if (!shooterTransform)
	{
		return;
	}

	auto position = shooterTransform->GetLocalPosition();
	auto bullet = std::make_unique<dae::GameObject>();
	bullet->AddComponent<dae::TransformComponent>();
	bullet->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x + 2.f, position.y - 10.f);

	bullet->AddComponent<dae::RenderComponent>("Sprites/BulletSprite.png");
	bullet->AddComponent<CollisionComponent>(4.f, 12.f);
	bullet->AddComponent<BulletComponent>(m_Scene, 400.f, missileLimit);

	auto* scoreComponent = m_Shooter.GetComponent<dae::ScoreComponent>();
	if (scoreComponent)
	{
		bullet->AddComponent<BulletEnemyCollisionComponent>(m_Scene, *scoreComponent, m_GameController);
	}

	if (missileLimit)
	{
		missileLimit->RegisterMissile();
	}

	m_Scene.Add(std::move(bullet));

	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::Shoot), 1.0f);
}