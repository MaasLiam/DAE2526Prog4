#include "MovePlayerCommand.h"

#include "GalagaGameControllerComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "GameState.h"
#include "TransformComponent.h"

MovePlayerCommand::MovePlayerCommand(dae::GameObject& object, dae::MoveDirection direction, float speed, GalagaGameControllerComponent& gameController, ControlledPlayer controlledPlayer)
	: m_Object(object)
	, m_Direction(direction)
	, m_Speed(speed)
	, m_GameController(gameController)
	, m_ControlledPlayer(controlledPlayer)
{}

void MovePlayerCommand::Execute(float deltaTime)
{
	if (m_GameController.GetState() != GameState::Playing)
	{
		return;
	}

	if (m_ControlledPlayer == ControlledPlayer::PlayerTwo &&
		m_GameController.GetGameMode() != GameMode::Coop)
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