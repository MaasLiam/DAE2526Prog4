#include "MissileLimitComponent.h"

MissileLimitComponent::MissileLimitComponent(dae::GameObject* owner)
    : dae::Component(owner)
{}

bool MissileLimitComponent::CanShoot() const
{
    return m_ActiveMissiles < m_MaxMissiles;
}

void MissileLimitComponent::RegisterMissile()
{
    if (CanShoot())
    {
        ++m_ActiveMissiles;
    }
}

void MissileLimitComponent::UnregisterMissile()
{
    if (m_ActiveMissiles > 0)
    {
        --m_ActiveMissiles;
    }
}