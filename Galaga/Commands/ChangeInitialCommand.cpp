#include "ChangeInitialCommand.h"

#include "GalagaGameControllerComponent.h"

ChangeInitialCommand::ChangeInitialCommand(GalagaGameControllerComponent& gameController, int direction)
	: m_GameController(gameController)
	, m_Direction(direction)
{}

void ChangeInitialCommand::Execute(float)
{
	m_GameController.ChangeSelectedInitial(m_Direction);
}