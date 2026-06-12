#include "MissileLimitComponent.h"

galaga::MissileLimitComponent::MissileLimitComponent(dae::GameObject* owner)
    : dae::Component(owner)
{}

bool galaga::MissileLimitComponent::CanShoot() const
{
    return m_ActiveMissiles < MaxMissiles;
}

void galaga::MissileLimitComponent::RegisterMissile()
{
    if (CanShoot())
    {
        ++m_ActiveMissiles;
    }
}

void galaga::MissileLimitComponent::UnregisterMissile()
{
    if (m_ActiveMissiles > 0)
    {
        --m_ActiveMissiles;
    }
}