#pragma once
#include "Component.h"
#include <memory>
#include "Subject.h"

namespace dae
{
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(GameObject* parent, int startLives = 3);

		void LoseLife();
		int GetLives() const;
		bool IsDead() const;

		Subject& GetSubject();
	private:
		int m_Lives;
		bool m_IsDead{ false };
		Subject m_Subject{};
	};
}