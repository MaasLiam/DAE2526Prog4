#pragma once

#include "Command.h"
#include "MoveObjectCommand.h"

namespace dae
{
	class GameObject;
}

class GalagaGameControllerComponent;

enum class ControlledPlayer
{
	PlayerOne,
	PlayerTwo
};

class MovePlayerCommand final : public dae::Command
{
public:
	MovePlayerCommand(dae::GameObject& object, dae::MoveDirection direction, float speed, GalagaGameControllerComponent& gameController, ControlledPlayer controlledPlayer);

	void Execute(float deltaTime) override;

private:
	dae::GameObject& m_Object;
	dae::MoveDirection m_Direction{};
	float m_Speed{};
	GalagaGameControllerComponent& m_GameController;
	ControlledPlayer m_ControlledPlayer{};
};