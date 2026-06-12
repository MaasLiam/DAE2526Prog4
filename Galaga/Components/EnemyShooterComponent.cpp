#include "EnemyShooterComponent.h"

#include "BulletComponent.h"
#include "CollisionComponent.h"
#include "EnemyBulletCollisionComponent.h"
#include "EnemyComponent.h"
#include "GalagaGameControllerComponent.h"
#include "GameObject.h"
#include "GameState.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "SoundIds.h"
#include "GameplayConstants.h"

#include <memory>

namespace
{
	constexpr float ShootDelayDuringDive{ 0.45f };
	constexpr float EnemyBulletSpeed{ -260.f };
}

galaga::EnemyShooterComponent::EnemyShooterComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_GameController(gameController)
	, m_Enemy(owner->GetComponent<EnemyComponent>())
	, m_Transform(owner->GetComponent<dae::TransformComponent>())
{

}

void galaga::EnemyShooterComponent::Update(float deltaTime)
{
	if (m_GameController.GetState() != galaga::GameState::Playing)
	{
		return;
	}

	if (m_Enemy == nullptr || m_Enemy->IsDead())
	{
		return;
	}

	if (m_Enemy->GetStateId() != galaga::EnemyStateId::Diving)
	{
		m_ShootTimer = 0.f;
		m_HasShotDuringCurrentDive = false;
		return;
	}

	if (m_HasShotDuringCurrentDive)
	{
		return;
	}

	m_ShootTimer += deltaTime;

	if (m_ShootTimer < ShootDelayDuringDive)
	{
		return;
	}

	Shoot();

	m_HasShotDuringCurrentDive = true;
}

void galaga::EnemyShooterComponent::Shoot()
{
	if (m_Transform == nullptr)
	{
		return;
	}

	const auto enemyPosition = m_Transform->GetLocalPosition();

	auto bullet = std::make_unique<dae::GameObject>();
	auto& bulletTransform = bullet->AddComponent<dae::TransformComponent>();
	bulletTransform.SetLocalPosition(enemyPosition.x + galaga::gameplay::EnemyBulletSpawnOffsetX, enemyPosition.y + galaga::gameplay::EnemyBulletSpawnOffsetY, 0.f);
	bullet->AddComponent<dae::RenderComponent>("Sprites/BulletSprite.png");
	bullet->AddComponent<CollisionComponent>(8.f, 16.f, galaga::gameplay::BulletHitboxOffsetX, galaga::gameplay::BulletHitboxOffsetY);
	bullet->AddComponent<BulletComponent>(m_Scene, EnemyBulletSpeed);
	bullet->AddComponent<EnemyBulletCollisionComponent>(m_Scene, m_GameController);

	m_Scene.Add(std::move(bullet));
}