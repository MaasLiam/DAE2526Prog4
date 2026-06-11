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

#include <memory>

namespace
{
	constexpr float ShootDelayDuringDive{ 0.45f };
	constexpr float EnemyBulletSpeed{ -260.f };
	constexpr float BulletOffsetX{ 14.f };
	constexpr float BulletOffsetY{ 28.f };
}

EnemyShooterComponent::EnemyShooterComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_GameController(gameController)
{

}

void EnemyShooterComponent::Update(float deltaTime)
{
	if (m_GameController.GetState() != GameState::Playing)
	{
		return;
	}

	auto* enemy = GetOwner()->GetComponent<EnemyComponent>();
	if (!enemy || enemy->IsDead())
	{
		return;
	}

	if (enemy->GetStateId() != EnemyStateId::Diving)
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

void EnemyShooterComponent::Shoot()
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!transform)
	{
		return;
	}

	const auto enemyPosition = transform->GetLocalPosition();

	auto bullet = std::make_unique<dae::GameObject>();
	bullet->AddComponent<dae::TransformComponent>();
	bullet->GetComponent<dae::TransformComponent>()->SetLocalPosition(enemyPosition.x + BulletOffsetX, enemyPosition.y + BulletOffsetY, 0.f);
	bullet->AddComponent<dae::RenderComponent>("Sprites/BulletSprite.png");
	bullet->AddComponent<CollisionComponent>(8.f, 16.f);
	bullet->AddComponent<BulletComponent>(m_Scene, EnemyBulletSpeed);
	bullet->AddComponent<EnemyBulletCollisionComponent>(m_Scene, m_GameController);

	m_Scene.Add(std::move(bullet));
}