#include "MoveInitialCursorCommand.h"

#include "GalagaGameControllerComponent.h"

MoveInitialCursorCommand::MoveInitialCursorCommand(GalagaGameControllerComponent& gameController, int direction)
	: m_GameController(gameController)
	, m_Direction(direction)
{}

void MoveInitialCursorCommand::Execute(float)
{
	m_GameController.MoveInitialCursor(m_Direction);
}