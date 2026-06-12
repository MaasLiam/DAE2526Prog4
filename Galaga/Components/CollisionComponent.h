#pragma once

#include "Component.h"

#include <SDL3/SDL_rect.h>

namespace dae
{
	class GameObject;
	class TransformComponent;
}

namespace galaga
{
	class CollisionComponent final : public dae::Component
	{
	public:
		CollisionComponent(dae::GameObject* owner, float width, float height, float offsetX = 0.f, float offsetY = 0.f);
		~CollisionComponent() override = default;

		CollisionComponent(const CollisionComponent&) = delete;
		CollisionComponent(CollisionComponent&&) = delete;
		CollisionComponent& operator=(const CollisionComponent&) = delete;
		CollisionComponent& operator=(CollisionComponent&&) = delete;

		[[nodiscard]] bool Overlaps(const CollisionComponent& other) const;
		[[nodiscard]] SDL_FRect GetBounds() const;

	private:
		dae::TransformComponent* m_Transform{};

		float m_Width{};
		float m_Height{};
		float m_OffsetX{};
		float m_OffsetY{};
	};
}