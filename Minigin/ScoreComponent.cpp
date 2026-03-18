#include "ScoreComponent.h"
#include "Event.h"

namespace dae
{
	ScoreComponent::ScoreComponent(GameObject* owner)
		: Component(owner)
	{
	}
	void ScoreComponent::AddScore(int score)
	{
		m_Score += score;
		m_Subject.Notify(Event::ScoreChanged, GetOwner());
	}
	int ScoreComponent::GetScore() const
	{
		return m_Score;
	}
	Subject& ScoreComponent::GetSubject()
	{
		return m_Subject;
	}
}