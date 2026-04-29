#include "BulletComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "Scene.h"

BulletComponent::BulletComponent(dae::GameObject* owner, dae::Scene& scene, float speed)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_Speed(speed)
{
}

void BulletComponent::Update(float deltaTime)
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!transform)
		return;

	auto position = transform->GetLocalPosition();

	position.y -= m_Speed * deltaTime;
	transform->SetLocalPosition(position);

	if (position.y < -32.f)
	{
		m_Scene.Remove(*GetOwner());
	}
}