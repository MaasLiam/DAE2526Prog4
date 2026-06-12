#pragma once

#include "Component.h"
#include "Subject.h"

namespace galaga
{
	class HealthComponent final : public dae::Component
	{
	public:
		HealthComponent(dae::GameObject* owner, int startLives = 3);

		~HealthComponent() override = default;

		HealthComponent(const HealthComponent&) = delete;
		HealthComponent(HealthComponent&&) = delete;
		HealthComponent& operator=(const HealthComponent&) = delete;
		HealthComponent& operator=(HealthComponent&&) = delete;

		void LoseLife();
		void Reset(int lives);

		[[nodiscard]] int GetLives() const;
		[[nodiscard]] bool IsDead() const;

		[[nodiscard]] dae::Subject& GetSubject();

	private:
		int m_Lives{};
		bool m_IsDead{};
		dae::Subject m_Subject{};
	};
}