#include "HealthComponent.h"
#include "Event.h"

galaga::HealthComponent::HealthComponent(dae::GameObject* parent, int startLives)
	: Component(parent)
	, m_Lives(startLives)
{}

void galaga::HealthComponent::LoseLife()
{
	if (m_Lives <= 0)
	{
		return;
	}

	--m_Lives;

	m_Subject.Notify(dae::Event::PlayerDied, GetOwner());

	if (m_Lives <= 0)
	{
		m_IsDead = true;
		m_Subject.Notify(dae::Event::GameOver, GetOwner());
	}
}

void galaga::HealthComponent::Reset(int lives)
{
	m_Lives = lives;
	m_IsDead = false;
	m_Subject.Notify(dae::Event::PlayerDied, GetOwner());
}

int galaga::HealthComponent::GetLives() const
{
	return m_Lives;
}

bool galaga::HealthComponent::IsDead() const
{
	return m_IsDead;
}

dae::Subject& galaga::HealthComponent::GetSubject()
{
	return m_Subject;
}
