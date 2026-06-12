#include "CollisionComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

galaga::CollisionComponent::CollisionComponent(dae::GameObject* owner,float width, float height, float offsetX, float offsetY)
	: dae::Component(owner)
	, m_Transform(owner->GetComponent<dae::TransformComponent>())
	, m_Width(width)
	, m_Height(height)
	, m_OffsetX(offsetX)
	, m_OffsetY(offsetY)
{

}

SDL_FRect galaga::CollisionComponent::GetBounds() const
{
	if (m_Transform == nullptr)
	{
		return SDL_FRect{ 0.f, 0.f, 0.f, 0.f };
	}

	const auto position = m_Transform->GetLocalPosition();

	return SDL_FRect{position.x + m_OffsetX, position.y + m_OffsetY, m_Width, m_Height};
}

bool galaga::CollisionComponent::Overlaps(const CollisionComponent& other) const
{
	const SDL_FRect a = GetBounds();
	const SDL_FRect b = other.GetBounds();

	return a.x < b.x + b.w &&
		a.x + a.w > b.x &&
		a.y < b.y + b.h &&
		a.y + a.h > b.y;
}