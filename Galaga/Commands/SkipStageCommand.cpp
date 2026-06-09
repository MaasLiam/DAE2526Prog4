#include "SkipStageCommand.h"

#include "GalagaGameControllerComponent.h"

SkipStageCommand::SkipStageCommand(GalagaGameControllerComponent& gameController)
	: m_GameController(gameController)
{}

void SkipStageCommand::Execute(float)
{
	m_GameController.SkipStage();
}