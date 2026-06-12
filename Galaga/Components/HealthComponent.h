#pragma once
#include "Component.h"
#include <memory>
#include "Subject.h"

namespace galaga
{
	class HealthComponent final : public dae::Component
	{
	public:
		HealthComponent(dae::GameObject* parent, int startLives = 3);

		void LoseLife();
		void Reset(int lives);
		int GetLives() const;
		bool IsDead() const;

		dae::Subject& GetSubject();
	private:
		int m_Lives;
		bool m_IsDead{ false };
		dae::Subject m_Subject{};
	};
}