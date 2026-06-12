#include "MoveInitialCursorCommand.h"

#include "GalagaGameControllerComponent.h"

galaga::MoveInitialCursorCommand::MoveInitialCursorCommand(GalagaGameControllerComponent& gameController, int direction)
	: m_GameController(gameController)
	, m_Direction(direction)
{}

void galaga::MoveInitialCursorCommand::Execute(float)
{
	m_GameController.MoveInitialCursor(m_Direction);
}