#include "EnemyFormationControllerComponent.h"

#include "EnemyComponent.h"
#include "Scene.h"
#include "GameObject.h"

#include <cstdlib>
#include <vector>

EnemyFormationControllerComponent::EnemyFormationControllerComponent(
    dae::GameObject* owner,
    dae::Scene& scene
)
    : dae::Component(owner)
    , m_Scene(scene)
{}

void EnemyFormationControllerComponent::Update(float deltaTime)
{
    m_DiveTimer += deltaTime;

    if (m_DiveTimer < m_TimeBetweenDives)
    {
        return;
    }

    m_DiveTimer = 0.f;

    std::vector<EnemyComponent*> availableEnemies{};

    for (const auto& object : m_Scene.GetObjects())
    {
        auto* enemy = object->GetComponent<EnemyComponent>();

        if (!enemy)
        {
            continue;
        }

        if (enemy->IsDead())
        {
            continue;
        }

        if (!enemy->IsInFormation())
        {
            continue;
        }

        availableEnemies.push_back(enemy);
    }

    if (availableEnemies.empty())
    {
        return;
    }

    const int randomIndex = rand() % static_cast<int>(availableEnemies.size());

    auto* selectedEnemy = availableEnemies[randomIndex];

    if (selectedEnemy->GetType() == galaga::EnemyType::BossGalaga && rand() % 2 == 0)
    {
        selectedEnemy->StartTractorBeam();
        return;
    }

    selectedEnemy->StartDiving();
}