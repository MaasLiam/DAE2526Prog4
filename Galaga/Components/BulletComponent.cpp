#include "BulletComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "MissileLimitComponent.h"

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
	if (m_MissileLimit)
	{
		m_MissileLimit->UnregisterMissile();
	}
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

	if (position.y < -32.f || position.y > 640.f)
	{
		m_Scene.Remove(*GetOwner());
	}
}