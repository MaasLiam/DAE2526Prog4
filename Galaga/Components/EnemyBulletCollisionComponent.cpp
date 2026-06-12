#include "EnemyBulletCollisionComponent.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SoundIds.h"

galaga::EnemyBulletCollisionComponent::EnemyBulletCollisionComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_GameController(gameController)
{

}

void galaga::EnemyBulletCollisionComponent::Update(float)
{
	if (TryHitPlayer(galaga::PlayerIndex::PlayerOne))
	{
		return;
	}

	TryHitPlayer(galaga::PlayerIndex::PlayerTwo);
}

bool galaga::EnemyBulletCollisionComponent::TryHitPlayer(galaga::PlayerIndex playerIndex)
{
	if (!m_GameController.IsPlayerActive(playerIndex) || !m_GameController.IsPlayerAlive(playerIndex))
	{
		return false;
	}

	auto* player = m_GameController.GetPlayer(playerIndex);
	if (!player)
	{
		return false;
	}

	auto* bulletCollision = GetOwner()->GetComponent<CollisionComponent>();
	auto* playerCollision = player->GetComponent<CollisionComponent>();
	auto* playerHealth = player->GetComponent<HealthComponent>();

	if (!bulletCollision || !playerCollision || !playerHealth)
	{
		return false;
	}

	if (!bulletCollision->Overlaps(*playerCollision))
	{
		return false;
	}

	playerHealth->LoseLife();
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);

	m_Scene.Remove(*GetOwner());

	return true;
}