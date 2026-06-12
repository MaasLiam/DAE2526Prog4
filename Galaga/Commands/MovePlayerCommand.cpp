#include "MovePlayerCommand.h"

#include "GalagaGameControllerComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "GameState.h"
#include "HealthComponent.h"
#include "TransformComponent.h"

namespace
{
	bool CanExecuteForContext(galaga::ControlContext controlContext, galaga::GameMode gameMode)
	{
		switch (controlContext)
		{
		case galaga::ControlContext::Always:
			return true;

		case galaga::ControlContext::SinglePlayerOnly:
			return gameMode == galaga::GameMode::SinglePlayer;

		case galaga::ControlContext::MultiplayerOnly:
			return gameMode != galaga::GameMode::SinglePlayer;
		}

		return false;
	}
}

galaga::MovePlayerCommand::MovePlayerCommand(dae::GameObject& object, dae::MoveDirection direction, float speed, GalagaGameControllerComponent& gameController, ControlledPlayer controlledPlayer, ControlContext controlContext)
	: m_Object(object)
	, m_Direction(direction)
	, m_Speed(speed)
	, m_GameController(gameController)
	, m_ControlledPlayer(controlledPlayer)
	, m_ControlContext(controlContext)
{}

void galaga::MovePlayerCommand::Execute(float deltaTime)
{
	if (!CanExecuteForContext(m_ControlContext, m_GameController.GetGameMode()))
	{
		return;
	}

	const galaga::PlayerIndex playerIndex = m_ControlledPlayer == ControlledPlayer::PlayerOne ? galaga::PlayerIndex::PlayerOne : galaga::PlayerIndex::PlayerTwo;

	if (!m_GameController.CanPlayerAct(playerIndex))
	{
		return;
	}

	auto* transform = m_Object.GetComponent<dae::TransformComponent>();
	if (!transform)
	{
		return;
	}

	glm::vec3 direction{};

	switch (m_Direction)
	{
	case dae::MoveDirection::Up:
		direction = { 0.f, -1.f, 0.f };
		break;
	case dae::MoveDirection::Down:
		direction = { 0.f, 1.f, 0.f };
		break;
	case dae::MoveDirection::Left:
		direction = { -1.f, 0.f, 0.f };
		break;
	case dae::MoveDirection::Right:
		direction = { 1.f, 0.f, 0.f };
		break;
	}

	const glm::vec3 currentPosition = transform->GetLocalPosition();
	transform->SetLocalPosition(currentPosition + direction * m_Speed * deltaTime);
}