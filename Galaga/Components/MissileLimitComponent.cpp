#include "MissileLimitComponent.h"

galaga::MissileLimitComponent::MissileLimitComponent(dae::GameObject* owner)
	: dae::Component(owner)
{}

bool galaga::MissileLimitComponent::CanShoot() const
{
	return m_ActiveMissileCount < MaxMissileCount;
}

int galaga::MissileLimitComponent::GetActiveMissileCount() const
{
	return m_ActiveMissileCount;
}

void galaga::MissileLimitComponent::RegisterMissile()
{
	if (!CanShoot())
	{
		return;
	}

	++m_ActiveMissileCount;
}

void galaga::MissileLimitComponent::UnregisterMissile()
{
	if (m_ActiveMissileCount <= 0)
	{
		return;
	}

	--m_ActiveMissileCount;
}

void galaga::MissileLimitComponent::Reset()
{
	m_ActiveMissileCount = 0;
}