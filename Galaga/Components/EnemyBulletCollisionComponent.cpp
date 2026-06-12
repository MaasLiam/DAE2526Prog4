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
	, m_BulletCollision(owner->GetComponent<CollisionComponent>())
{

}

void galaga::EnemyBulletCollisionComponent::Update(float)
{
	if (m_BulletCollision == nullptr)
	{
		return;
	}

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

	if (player == nullptr)
	{
		return false;
	}

	auto* playerCollision = player->GetComponent<CollisionComponent>();
	auto* playerHealth = player->GetComponent<HealthComponent>();

	if (playerCollision == nullptr || playerHealth == nullptr)
	{
		return false;
	}

	if (!m_BulletCollision->Overlaps(*playerCollision))
	{
		return false;
	}

	playerHealth->LoseLife();
	m_GameController.RespawnPlayer(playerIndex);
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);

	m_Scene.Remove(*GetOwner());

	return true;
}