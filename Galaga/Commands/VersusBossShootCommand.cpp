#include "VersusBossShootCommand.h"

#include "BulletComponent.h"
#include "CollisionComponent.h"
#include "GalagaGameControllerComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "GameState.h"
#include "PlayerBulletCollisionComponent.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "SoundIds.h"
#include "GameplayConstants.h"

galaga::VersusBossShootCommand::VersusBossShootCommand(dae::GameObject& boss, dae::GameObject& targetPlayer, dae::Scene& scene, GalagaGameControllerComponent& gameController)
	: m_Boss(boss)
	, m_TargetPlayer(targetPlayer)
	, m_Scene(scene)
	, m_GameController(gameController)
{}

void galaga::VersusBossShootCommand::Execute(float)
{
	if (m_GameController.GetState() != galaga::GameState::Playing || m_GameController.GetGameMode() != galaga::GameMode::Versus)
	{
		return;
	}

	auto* bossTransform = m_Boss.GetComponent<dae::TransformComponent>();
	if (!bossTransform)
	{
		return;
	}

	const auto bossPosition = bossTransform->GetLocalPosition();

	auto bullet = std::make_unique<dae::GameObject>();
	bullet->AddComponent<dae::TransformComponent>();
	bullet->GetComponent<dae::TransformComponent>()->SetLocalPosition(bossPosition.x + galaga::gameplay::EnemyBulletSpawnOffsetX, bossPosition.y + galaga::gameplay::EnemyBulletSpawnOffsetY, 0.f);

	bullet->AddComponent<dae::RenderComponent>("Sprites/BulletSprite.png");
	bullet->AddComponent<CollisionComponent>(8.f, 16.f, galaga::gameplay::BulletHitboxOffsetX, galaga::gameplay::BulletHitboxOffsetY);

	// Negative speed means BulletComponent moves downward because it does y -= speed * dt.
	bullet->AddComponent<BulletComponent>(m_Scene, -260.f);
	bullet->AddComponent<PlayerBulletCollisionComponent>(m_Scene, m_TargetPlayer);

	m_Scene.Add(std::move(bullet));
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::Shoot), 1.0f);
}