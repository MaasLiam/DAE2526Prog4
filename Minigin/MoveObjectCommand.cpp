#include "MoveObjectCommand.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <glm/vec3.hpp>

dae::MoveObjectCommand::MoveObjectCommand(GameObject& object, MoveDirection direction, float speed)
	: m_Object(object)
	, m_Direction(direction)
	, m_Speed(speed)
{
}

void dae::MoveObjectCommand::Execute(float deltaTime)
{
	auto* transform = m_Object.GetComponent<TransformComponent>();
	if (!transform)
		return;

	glm::vec3 direction{};

	switch (m_Direction)
	{
	case MoveDirection::Up:
		direction = { 0.f, -1.f, 0.f };
		break;
	case MoveDirection::Down:
		direction = { 0.f, 1.f, 0.f };
		break;
	case MoveDirection::Left:
		direction = { -1.f, 0.f, 0.f };
		break;
	case MoveDirection::Right:
		direction = { 1.f, 0.f, 0.f };
		break;
	}

	const glm::vec3 currentPos = transform->GetLocalPosition();
	transform->SetLocalPosition(currentPos + direction * m_Speed * deltaTime);
}