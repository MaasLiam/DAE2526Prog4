#include "AddScoreCommand.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include "Event.h"

namespace dae
{
	AddScoreCommand::AddScoreCommand(GameObject& target)
		: m_Target(target)
	{
	}

	void AddScoreCommand::Execute(float)
	{
		auto* scoreComponent = m_Target.GetComponent<ScoreComponent>();
		if (!scoreComponent)
		{
			return;
		}

		scoreComponent->Notify(Event::EnemyKilled, &m_Target);
	}
}