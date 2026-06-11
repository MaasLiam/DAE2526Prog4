#include "HealthComponent.h"
#include "Event.h"

namespace dae
{
	HealthComponent::HealthComponent(GameObject* parent, int startLives)
		: Component(parent)
		, m_Lives(startLives)
	{
	}

	void HealthComponent::LoseLife()
	{
		if (m_Lives <= 0)
		{
			return;
		}

		--m_Lives;

		m_Subject.Notify(Event::PlayerDied, GetOwner());

		if (m_Lives <= 0)
		{
			m_IsDead = true;
			m_Subject.Notify(Event::GameOver, GetOwner());
		}
	}

	void HealthComponent::Reset(int lives)
	{
		m_Lives = lives;
		m_IsDead = false;
		m_Subject.Notify(Event::PlayerDied, GetOwner());
	}

	int HealthComponent::GetLives() const
	{
		return m_Lives;
	}

	bool HealthComponent::IsDead() const
	{
		return m_IsDead;
	}

	Subject& HealthComponent::GetSubject()
	{
		return m_Subject;
	}
}
