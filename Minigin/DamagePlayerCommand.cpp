#include "DamagePlayerCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"

namespace dae
{
	DamagePlayerCommand::DamagePlayerCommand(GameObject& player)
		: m_Player(player)
	{
	}

	void DamagePlayerCommand::Execute(float)
	{
		if (auto* health = m_Player.GetComponent<HealthComponent>())
		{
			health->LoseLife();
		}
	}
}