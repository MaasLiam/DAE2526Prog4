#include "RotationComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

dae::RotationComponent::RotationComponent(GameObject* owner, float radius, float angularSpeed, const glm::vec3& center)
	: Component(owner)
	, m_Radius(radius)
	, m_AngularSpeed(angularSpeed)
	, m_Center(center)
{

}

void dae::RotationComponent::Update(float deltaTime)
{
	auto* transform = GetOwner()->GetComponent<TransformComponent>();
	if (!transform) return;

	m_Angle += m_AngularSpeed * deltaTime;

	const glm::vec3 offset{ cos(m_Angle) * m_Radius, sin(m_Angle) * m_Radius, 0.f };
	transform->SetLocalPosition(offset + m_Center);
}
