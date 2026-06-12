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
	, m_BulletCollision(owner->GetComponent<CollisionComponent>())
	, m_PlayerCollision(player.GetComponent<CollisionComponent>())
	, m_PlayerHealth(player.GetComponent<HealthComponent>())
{

}

void galaga::PlayerBulletCollisionComponent::Update(float)
{
	if (m_BulletCollision == nullptr || m_PlayerCollision == nullptr || m_PlayerHealth == nullptr)
	{
		return;
	}

	if (m_PlayerHealth->IsDead())
	{
		return;
	}

	if (!m_BulletCollision->Overlaps(*m_PlayerCollision))
	{
		return;
	}

	m_PlayerHealth->LoseLife();
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);

	m_Scene.Remove(*GetOwner());
}