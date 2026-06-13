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
#include "GameplayConstants.h"

namespace
{
	bool CanExecuteForContext(galaga::ControlContext controlContext, galaga::GameMode gameMode)
	{
		switch (controlContext)
		{
		case galaga::ControlContext::Always:
			return true;

		case galaga::ControlContext::SinglePlayerOnly:
			return gameMode == galaga::GameMode::SinglePlayer;

		case galaga::ControlContext::MultiplayerOnly:
			return gameMode != galaga::GameMode::SinglePlayer;
		}

		return false;
	}
}

galaga::ShootCommand::ShootCommand(
	dae::GameObject& shooter,
	dae::Scene& scene,
	GalagaGameControllerComponent* gameController,
	ShootOwner owner,
	ControlContext controlContext
)
	: m_Scene(scene)
	, m_GameController(gameController)
	, m_MissileLimit(shooter.GetComponent<MissileLimitComponent>())
	, m_ShooterTransform(shooter.GetComponent<dae::TransformComponent>())
	, m_ScoreComponent(shooter.GetComponent<galaga::ScoreComponent>())
	, m_Owner(owner)
	, m_ControlContext(controlContext)
{

}

void galaga::ShootCommand::Execute(float)
{
	if (!CanShootForCurrentState())
	{
		return;
	}

	SpawnBullet();

	m_MissileLimit->RegisterMissile();

	if (m_GameController != nullptr)
	{
		m_GameController->RegisterShotFired();
	}

	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::Shoot), 1.0f);
}

bool galaga::ShootCommand::CanShootForCurrentState() const
{
	if (m_GameController != nullptr && m_GameController->GetState() != galaga::GameState::Playing)
	{
		return false;
	}

	if (m_GameController != nullptr && !CanExecuteForContext(m_ControlContext, m_GameController->GetGameMode()))
	{
		return false;
	}

	if (m_GameController != nullptr && m_Owner == ShootOwner::PlayerTwo && m_GameController->GetGameMode() == galaga::GameMode::Versus)
	{
		return false;
	}

	if (m_GameController != nullptr)
	{
		const auto playerIndex = m_Owner == ShootOwner::PlayerOne ? galaga::PlayerIndex::PlayerOne : galaga::PlayerIndex::PlayerTwo;

		if (!m_GameController->CanPlayerAct(playerIndex))
		{
			return false;
		}
	}

	return m_MissileLimit != nullptr && m_MissileLimit->CanShoot() && m_ShooterTransform != nullptr;
}

void galaga::ShootCommand::SpawnBullet()
{
	const auto position = m_ShooterTransform->GetLocalPosition();

	auto bullet = std::make_unique<dae::GameObject>();

	auto& bulletTransform = bullet->AddComponent<dae::TransformComponent>();
	bulletTransform.SetLocalPosition(position.x + galaga::gameplay::PlayerBulletSpawnOffsetX, position.y + galaga::gameplay::PlayerBulletSpawnOffsetY, 0.f);
	bullet->AddComponent<dae::RenderComponent>("Sprites/BulletSprite.png");
	bullet->AddComponent<CollisionComponent>(4.f, 12.f);
	bullet->AddComponent<BulletComponent>(m_Scene, 400.f, m_MissileLimit);

	if (m_ScoreComponent != nullptr)
	{
		bullet->AddComponent<BulletEnemyCollisionComponent>(m_Scene, *m_ScoreComponent, m_GameController);
	}

	m_Scene.Add(std::move(bullet));
}