#include "EnemyPlayerCollisionComponent.h"

#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SoundIds.h"

#include <algorithm>

galaga::EnemyPlayerCollisionComponent::EnemyPlayerCollisionComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_GameController(gameController)
{

}

void galaga::EnemyPlayerCollisionComponent::AddPlayer(dae::GameObject* player)
{
	if (player == nullptr)
	{
		return;
	}

	const auto isAlreadyRegistered = std::find(m_Players.begin(), m_Players.end(), player) != m_Players.end();

	if (isAlreadyRegistered)
	{
		return;
	}

	m_Players.emplace_back(player);
}

void galaga::EnemyPlayerCollisionComponent::Update(float)
{
	for (const auto& object : m_Scene.GetObjects())
	{
		if (object == nullptr)
		{
			continue;
		}

		if (TryHandleEnemyCollision(*object))
		{
			return;
		}
	}
}

bool galaga::EnemyPlayerCollisionComponent::TryHandleEnemyCollision(dae::GameObject& object)
{
	auto* enemy = object.GetComponent<EnemyComponent>();
	auto* enemyCollision = object.GetComponent<CollisionComponent>();

	if (enemy == nullptr || enemyCollision == nullptr || enemy->IsDead())
	{
		return false;
	}

	for (size_t playerIndex{}; playerIndex < m_Players.size(); ++playerIndex)
	{
		auto* player = m_Players[playerIndex];

		if (player == nullptr)
		{
			continue;
		}

		const auto activePlayerIndex = playerIndex == 0 ? galaga::PlayerIndex::PlayerOne : galaga::PlayerIndex::PlayerTwo;

		if (TryHandlePlayerCollision(*enemy, *enemyCollision, *player, activePlayerIndex, object))
		{
			return true;
		}
	}

	return false;
}

bool galaga::EnemyPlayerCollisionComponent::TryHandlePlayerCollision(EnemyComponent& enemy, CollisionComponent& enemyCollision, dae::GameObject& player, PlayerIndex playerIndex, dae::GameObject& enemyObject)
{
	if (!m_GameController.IsPlayerActive(playerIndex))
	{
		return false;
	}

	auto* playerCollision = player.GetComponent<CollisionComponent>();
	auto* playerHealth = player.GetComponent<HealthComponent>();

	if (playerCollision == nullptr || playerHealth == nullptr || playerHealth->IsDead())
	{
		return false;
	}

	if (!enemyCollision.Overlaps(*playerCollision))
	{
		return false;
	}

	playerHealth->LoseLife();
	m_GameController.RespawnPlayer(playerIndex);
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);

	enemy.TakeDamage();

	if (enemy.IsDead())
	{
		m_Scene.Remove(enemyObject);
	}

	return true;
}