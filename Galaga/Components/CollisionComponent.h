#pragma once

#include "Component.h"

namespace galaga
{
	class CollisionComponent final : public dae::Component
	{
	public:
		CollisionComponent(dae::GameObject* owner, float width, float height);

		bool Overlaps(const CollisionComponent& other) const;

		float GetWidth() const;
		float GetHeight() const;

	private:
		float m_Width{};
		float m_Height{};
	};
}