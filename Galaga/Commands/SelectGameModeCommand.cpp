#include "SelectGameModeCommand.h"

#include "GalagaGameControllerComponent.h"

namespace galaga
{
	SelectGameModeCommand::SelectGameModeCommand(GalagaGameControllerComponent& gameController, GameMode gameMode)
		: m_GameController(gameController)
		, m_GameMode(gameMode)
	{
	
	}

	void SelectGameModeCommand::Execute(float)
	{
		m_GameController.SelectGameMode(m_GameMode);
	}
}