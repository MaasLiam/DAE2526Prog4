#include "PlayerBulletCollisionComponent.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SoundIds.h"

galaga::PlayerBulletCollisionComponent::PlayerBulletCollisionComponent(dae::GameObject* owner, dae::Scene& scene, dae::GameObject& player)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_Player(player)
{

}

void galaga::PlayerBulletCollisionComponent::Update(float)
{
	auto* bulletCollision = GetOwner()->GetComponent<CollisionComponent>();
	auto* playerCollision = m_Player.GetComponent<CollisionComponent>();
	auto* playerHealth = m_Player.GetComponent<HealthComponent>();

	if (!bulletCollision || !playerCollision || !playerHealth || playerHealth->IsDead())
	{
		return;
	}

	if (!bulletCollision->Overlaps(*playerCollision))
	{
		return;
	}

	playerHealth->LoseLife();
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);
	m_Scene.Remove(*GetOwner());
}