#include "BulletComponent.h"

#include "GameObject.h"
#include "MissileLimitComponent.h"
#include "Scene.h"
#include "TransformComponent.h"

galaga::BulletComponent::BulletComponent(dae::GameObject* owner, dae::Scene& scene, float speed, MissileLimitComponent* missileLimit)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_Transform(owner->GetComponent<dae::TransformComponent>())
	, m_MissileLimit(missileLimit)
	, m_Speed(speed)
{

}

galaga::BulletComponent::~BulletComponent()
{
	if (m_MissileLimit == nullptr)
	{
		return;
	}

	m_MissileLimit->UnregisterMissile();
}

void galaga::BulletComponent::Update(float deltaTime)
{
	if (m_Transform == nullptr)
	{
		return;
	}

	auto position = m_Transform->GetLocalPosition();

	position.y -= m_Speed * deltaTime;
	m_Transform->SetLocalPosition(position);

	if (IsOutOfBounds(position.y))
	{
		m_Scene.Remove(*GetOwner());
	}
}

bool galaga::BulletComponent::IsOutOfBounds(float yPosition) const
{
	return yPosition < MinimumYPosition || yPosition > MaximumYPosition;
}