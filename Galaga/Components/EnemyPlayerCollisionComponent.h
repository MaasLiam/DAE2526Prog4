#pragma once

#include "Component.h"
#include <vector>

namespace dae
{
    class GameObject;
    class Scene;
}

class EnemyPlayerCollisionComponent final : public dae::Component
{
public:
    EnemyPlayerCollisionComponent(dae::GameObject* owner, dae::Scene& scene);

    void Update(float deltaTime) override;

    void AddPlayer(dae::GameObject* player);

private:
    dae::Scene& m_Scene;
    std::vector<dae::GameObject*> m_Players;
};