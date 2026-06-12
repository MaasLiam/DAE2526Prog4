#pragma once

#include "Component.h"

namespace galaga
{
	class MissileLimitComponent final : public dae::Component
	{
	public:
		explicit MissileLimitComponent(dae::GameObject* owner);

		~MissileLimitComponent() override = default;

		MissileLimitComponent(const MissileLimitComponent&) = delete;
		MissileLimitComponent(MissileLimitComponent&&) = delete;
		MissileLimitComponent& operator=(const MissileLimitComponent&) = delete;
		MissileLimitComponent& operator=(MissileLimitComponent&&) = delete;

		[[nodiscard]] bool CanShoot() const;
		[[nodiscard]] int GetActiveMissileCount() const;

		void RegisterMissile();
		void UnregisterMissile();
		void Reset();

	private:
		static constexpr int MaxMissileCount{ 2 };

		int m_ActiveMissileCount{};
	};
}