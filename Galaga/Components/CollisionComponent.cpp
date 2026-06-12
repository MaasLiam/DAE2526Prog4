#include "CollisionComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

galaga::CollisionComponent::CollisionComponent(dae::GameObject* owner, float width, float height)
	: dae::Component(owner)
	, m_Width(width)
	, m_Height(height)
{
}

bool galaga::CollisionComponent::Overlaps(const CollisionComponent& other) const
{
	auto* myTransform = GetOwner()->GetComponent<dae::TransformComponent>();
	auto* otherTransform = other.GetOwner()->GetComponent<dae::TransformComponent>();

	if (!myTransform || !otherTransform)
		return false;

	const auto myPos = myTransform->GetWorldPosition();
	const auto otherPos = otherTransform->GetWorldPosition();

	return myPos.x < otherPos.x + other.m_Width &&
		myPos.x + m_Width > otherPos.x &&
		myPos.y < otherPos.y + other.m_Height &&
		myPos.y + m_Height > otherPos.y;
}

float galaga::CollisionComponent::GetWidth() const
{
	return m_Width;
}

float galaga::CollisionComponent::GetHeight() const
{
	return m_Height;
}