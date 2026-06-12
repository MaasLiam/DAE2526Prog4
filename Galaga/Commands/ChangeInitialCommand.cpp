#include "ChangeInitialCommand.h"

#include "GalagaGameControllerComponent.h"

galaga::ChangeInitialCommand::ChangeInitialCommand(GalagaGameControllerComponent& gameController, int direction)
	: m_GameController(gameController)
	, m_Direction(direction)
{}

void galaga::ChangeInitialCommand::Execute(float)
{
	m_GameController.ChangeSelectedInitial(m_Direction);
}