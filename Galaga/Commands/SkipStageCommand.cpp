#include "SkipStageCommand.h"

#include "GalagaGameControllerComponent.h"

galaga::SkipStageCommand::SkipStageCommand(GalagaGameControllerComponent& gameController)
	: m_GameController(gameController)
{}

void galaga::SkipStageCommand::Execute(float)
{
	m_GameController.SkipStage();
}