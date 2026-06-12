#pragma once

#include "Component.h"
#include <SDL3/SDL_rect.h>

namespace galaga
{
	class CollisionComponent final : public dae::Component
	{
	public:
		CollisionComponent(dae::GameObject* owner, float width, float height, float offsetX = 0.f, float offsetY = 0.f);

		bool Overlaps(const CollisionComponent& other) const;
		SDL_FRect GetBounds() const;

	private:
		float m_Width{};
		float m_Height{};
		float m_OffsetX{};
		float m_OffsetY{};
	};
}