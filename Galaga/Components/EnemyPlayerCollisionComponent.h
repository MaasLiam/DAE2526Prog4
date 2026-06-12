#pragma once

#include "Component.h"
#include <vector>

namespace dae
{
    class GameObject;
    class Scene;
}

namespace galaga
{
    class GalagaGameControllerComponent;
    class EnemyPlayerCollisionComponent final : public dae::Component
    {
    public:
        EnemyPlayerCollisionComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController);

        void Update(float deltaTime) override;

        void AddPlayer(dae::GameObject* player);

        ~EnemyPlayerCollisionComponent() override = default;

        EnemyPlayerCollisionComponent(const EnemyPlayerCollisionComponent&) = delete;
        EnemyPlayerCollisionComponent(EnemyPlayerCollisionComponent&&) = delete;
        EnemyPlayerCollisionComponent& operator=(const EnemyPlayerCollisionComponent&) = delete;
        EnemyPlayerCollisionComponent& operator=(EnemyPlayerCollisionComponent&&) = delete;

    private:
        dae::Scene& m_Scene;
        std::vector<dae::GameObject*> m_Players;
        GalagaGameControllerComponent& m_GameController;
    };
}