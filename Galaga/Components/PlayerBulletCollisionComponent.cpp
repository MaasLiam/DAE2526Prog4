#include "PlayerBulletCollisionComponent.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"

PlayerBulletCollisionComponent::PlayerBulletCollisionComponent(dae::GameObject* owner, dae::Scene& scene, dae::GameObject& player)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_Player(player)
{

}

void PlayerBulletCollisionComponent::Update(float)
{
	auto* bulletCollision = GetOwner()->GetComponent<CollisionComponent>();
	auto* playerCollision = m_Player.GetComponent<CollisionComponent>();
	auto* playerHealth = m_Player.GetComponent<dae::HealthComponent>();

	if (!bulletCollision || !playerCollision || !playerHealth || playerHealth->IsDead())
	{
		return;
	}

	if (!bulletCollision->Overlaps(*playerCollision))
	{
		return;
	}

	playerHealth->LoseLife();
	m_Scene.Remove(*GetOwner());
}