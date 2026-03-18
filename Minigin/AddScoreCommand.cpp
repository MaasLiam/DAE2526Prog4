#include "AddScoreCommand.h"
#include "GameObject.h"
#include "ScoreComponent.h"

namespace dae
{
	AddScoreCommand::AddScoreCommand(GameObject& target, int scoreToAdd)
		: m_Target(target)
		, m_ScoreToAdd(scoreToAdd)
	{
	}
	void AddScoreCommand::Execute(float)
	{
		auto* scoreComponent = m_Target.GetComponent<ScoreComponent>();
		if (!scoreComponent)
		{
			return;
		}
		scoreComponent->AddScore(m_ScoreToAdd);
	}
}