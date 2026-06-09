#pragma once

#include "Component.h"

namespace dae
{
    class GameObject;
    class Scene;
}

class EnemyFormationControllerComponent final : public dae::Component
{
public:
    EnemyFormationControllerComponent(dae::GameObject* owner, dae::Scene& scene);

    void Update(float deltaTime) override;

private:
    dae::Scene& m_Scene;

    float m_DiveTimer{};
    float m_TimeBetweenDives{ 3.f };
};