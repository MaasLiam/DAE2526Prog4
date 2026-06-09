#include "EnemyPlayerCollisionComponent.h"

#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"

EnemyPlayerCollisionComponent::EnemyPlayerCollisionComponent(
    dae::GameObject* owner,
    dae::Scene& scene
)
    : dae::Component(owner)
    , m_Scene(scene)
{}

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

        for (auto* player : m_Players)
        {
            if (!player)
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

            enemy->TakeDamage();

            if (enemy->IsDead())
            {
                m_Scene.Remove(*object);
            }

            return;
        }
    }
}