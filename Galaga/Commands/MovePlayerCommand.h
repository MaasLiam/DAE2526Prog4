#pragma once

#include "Command.h"
#include "MoveObjectCommand.h"
#include "ControlContext.h"

namespace dae
{
	class GameObject;
}

namespace galaga
{
	class GalagaGameControllerComponent;
	enum class ControlledPlayer
	{
		PlayerOne,
		PlayerTwo
	};

	class MovePlayerCommand final : public dae::Command
	{
	public:
		MovePlayerCommand(dae::GameObject& object, dae::MoveDirection direction, float speed, GalagaGameControllerComponent& gameController, ControlledPlayer controlledPlayer, ControlContext controlContext = ControlContext::Always);
		~MovePlayerCommand() override = default;

		MovePlayerCommand(const MovePlayerCommand&) = delete;
		MovePlayerCommand(MovePlayerCommand&&) = delete;
		MovePlayerCommand& operator=(const MovePlayerCommand&) = delete;
		MovePlayerCommand& operator=(MovePlayerCommand&&) = delete;

		void Execute(float deltaTime) override;

	private:
		dae::GameObject& m_Object;
		dae::MoveDirection m_Direction{};
		float m_Speed{};
		GalagaGameControllerComponent& m_GameController;
		ControlledPlayer m_ControlledPlayer{};
		ControlContext m_ControlContext{ ControlContext::Always };
	};
}