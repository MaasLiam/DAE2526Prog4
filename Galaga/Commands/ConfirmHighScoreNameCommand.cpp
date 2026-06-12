#include "ConfirmHighScoreNameCommand.h"

#include "GalagaGameControllerComponent.h"

galaga::ConfirmHighScoreNameCommand::ConfirmHighScoreNameCommand(GalagaGameControllerComponent& gameController)
	: m_GameController(gameController)
{}

void galaga::ConfirmHighScoreNameCommand::Execute(float)
{
	m_GameController.ConfirmCurrentSelection();
}