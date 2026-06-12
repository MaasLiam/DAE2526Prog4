#pragma once

#include "Component.h"

namespace galaga
{
    class MissileLimitComponent final : public dae::Component
    {
    public:
        explicit MissileLimitComponent(dae::GameObject* owner);

        ~MissileLimitComponent() override = default;

        MissileLimitComponent(const MissileLimitComponent&) = delete;
        MissileLimitComponent(MissileLimitComponent&&) = delete;
        MissileLimitComponent& operator=(const MissileLimitComponent&) = delete;
        MissileLimitComponent& operator=(MissileLimitComponent&&) = delete;

        bool CanShoot() const;
        void RegisterMissile();
        void UnregisterMissile();

    private:
        int m_ActiveMissiles{};
        static constexpr int MaxMissiles{ 2 };
    };
}