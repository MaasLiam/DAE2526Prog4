#pragma once

#include "Component.h"

namespace galaga
{
    class MissileLimitComponent final : public dae::Component
    {
    public:
        explicit MissileLimitComponent(dae::GameObject* owner);

        bool CanShoot() const;
        void RegisterMissile();
        void UnregisterMissile();

    private:
        int m_ActiveMissiles{};
        static constexpr int MaxMissiles{ 2 };
    };
}