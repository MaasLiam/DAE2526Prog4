#include "ConfirmHighScoreNameCommand.h"

#include "GalagaGameControllerComponent.h"

ConfirmHighScoreNameCommand::ConfirmHighScoreNameCommand(GalagaGameControllerComponent& gameController)
	: m_GameController(gameController)
{}

void ConfirmHighScoreNameCommand::Execute(float)
{
	m_GameController.ConfirmHighScoreName();
}