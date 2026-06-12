#include "CapturedFighterComponent.h"

#include "BulletComponent.h"
#include "CollisionComponent.h"
#include "EnemyBulletCollisionComponent.h"
#include "GalagaGameControllerComponent.h"
#include "GameObject.h"
#include "GameState.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "TransformComponent.h"

#include <memory>

namespace
{
	constexpr float ShootInterval{ 1.25f };
	constexpr float BulletSpeed{ -260.f };
	constexpr float BulletOffsetX{ 14.f };
	constexpr float BulletOffsetY{ 28.f };
}

galaga::CapturedFighterComponent::CapturedFighterComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_GameController(gameController)
{}

void galaga::CapturedFighterComponent::Update(float deltaTime)
{
	if (m_GameController.GetState() != galaga::GameState::Playing)
	{
		return;
	}

	m_ShootTimer += deltaTime;

	if (m_ShootTimer < ShootInterval)
	{
		return;
	}

	m_ShootTimer = 0.f;
	Shoot();
}

void galaga::CapturedFighterComponent::Shoot()
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!transform)
	{
		return;
	}

	const auto position = transform->GetLocalPosition();

	auto bullet = std::make_unique<dae::GameObject>();
	bullet->AddComponent<dae::TransformComponent>();
	bullet->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x + BulletOffsetX, position.y + BulletOffsetY, 0.f);
	bullet->AddComponent<dae::RenderComponent>("Sprites/BulletSprite.png");
	bullet->AddComponent<CollisionComponent>(8.f, 16.f);
	bullet->AddComponent<BulletComponent>(m_Scene, BulletSpeed);
	bullet->AddComponent<EnemyBulletCollisionComponent>(m_Scene, m_GameController);

	m_Scene.Add(std::move(bullet));
}