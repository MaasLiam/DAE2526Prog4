#include "ScoreComponent.h"
#include "Event.h"

galaga::ScoreComponent::ScoreComponent(dae::GameObject* owner)
	: Component(owner)
{}

void galaga::ScoreComponent::Notify(dae::Event event, dae::GameObject*)
{
	if (event == dae::Event::EnemyKilled)
	{
		AddScore(100);
	}
}

void galaga::ScoreComponent::AddScore(int score)
{
	m_Score += score;
	m_Subject.Notify(dae::Event::ScoreChanged, GetOwner());
}

void galaga::ScoreComponent::Reset()
{
	m_Score = 0;
	m_Subject.Notify(dae::Event::ScoreChanged, GetOwner());
}

int galaga::ScoreComponent::GetScore() const
{
	return m_Score;
}

dae::Subject& galaga::ScoreComponent::GetSubject()
{
	return m_Subject;
}