#pragma once
#include "Command.h"
#include <glm/vec2.hpp>

namespace dae
{
	class GameObject;

	enum class MoveDirection
	{
		Up,
		Down,
		Left,
		Right
	};

	class MoveObjectCommand final : public Command
	{
	public:
		MoveObjectCommand(GameObject& object, MoveDirection direction, float speed);

		void Execute(float deltaTime) override;

	private:
		GameObject& m_Object;
		MoveDirection m_Direction;
		float m_Speed{};
	};
}