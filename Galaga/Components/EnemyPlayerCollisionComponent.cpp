#include "EnemyPlayerCollisionComponent.h"

#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "GalagaGameControllerComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SoundIds.h"

EnemyPlayerCollisionComponent::EnemyPlayerCollisionComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController)
    : dae::Component(owner)
    , m_Scene(scene)
    , m_GameController(gameController)
{

}

void EnemyPlayerCollisionComponent::AddPlayer(dae::GameObject* player)
{
    if (player)
    {
        m_Players.push_back(player);
    }
}

void EnemyPlayerCollisionComponent::Update(float)
{
    for (const auto& object : m_Scene.GetObjects())
    {
        auto* enemy = object->GetComponent<EnemyComponent>();
        auto* enemyCollision = object->GetComponent<CollisionComponent>();

        if (!enemy || !enemyCollision || enemy->IsDead())
        {
            continue;
        }

        for (size_t playerIndex{}; playerIndex < m_Players.size(); ++playerIndex)
        {
            auto* player = m_Players[playerIndex];

            if (!player)
            {
                continue;
            }

            const galaga::PlayerIndex activePlayerIndex = playerIndex == 0 ? galaga::PlayerIndex::PlayerOne : galaga::PlayerIndex::PlayerTwo;

            if (!m_GameController.IsPlayerActive(activePlayerIndex))
            {
                continue;
            }

            auto* playerCollision = player->GetComponent<CollisionComponent>();
            auto* playerHealth = player->GetComponent<dae::HealthComponent>();

            if (!playerCollision || !playerHealth || playerHealth->IsDead())
            {
                continue;
            }

            if (!enemyCollision->Overlaps(*playerCollision))
            {
                continue;
            }

            playerHealth->LoseLife();
            dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);

            enemy->TakeDamage();

            if (enemy->IsDead())
            {
                m_Scene.Remove(*object);
            }

            return;
        }
    }
}