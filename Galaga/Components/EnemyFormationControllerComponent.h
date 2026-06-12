#pragma once

#include "Component.h"

namespace dae
{
    class GameObject;
    class Scene;
}

namespace galaga
{
    class EnemyFormationControllerComponent final : public dae::Component
    {
    public:
        EnemyFormationControllerComponent(dae::GameObject* owner, dae::Scene& scene);

        void Update(float deltaTime) override;

        ~EnemyFormationControllerComponent() override = default;

        EnemyFormationControllerComponent(const EnemyFormationControllerComponent&) = delete;
        EnemyFormationControllerComponent(EnemyFormationControllerComponent&&) = delete;
        EnemyFormationControllerComponent& operator=(const EnemyFormationControllerComponent&) = delete;
        EnemyFormationControllerComponent& operator=(EnemyFormationControllerComponent&&) = delete;

    private:
        dae::Scene& m_Scene;

        float m_DiveTimer{};
        float m_TimeBetweenDives{ 3.f };
    };
}