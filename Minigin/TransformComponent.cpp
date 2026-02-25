#include "TransformComponent.h"
#include "GameObject.h"



dae::TransformComponent::TransformComponent(GameObject* parent)
	: Component(parent)
	, m_LocalPosition{}
	, m_WorldPosition{}
{
}

void dae::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	SetDirtyWorldPosition();
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	SetDirtyWorldPosition();
}

const glm::vec3& dae::TransformComponent::GetWorldPosition() const
{
	if (m_DirtyWorldPosition)
	{
		if (GetOwner()->GetParent() != nullptr)
		{
			TransformComponent* parentTransform{ GetOwner()->GetParent()->GetComponent<TransformComponent>()};
			m_WorldPosition = parentTransform->GetWorldPosition() + m_LocalPosition;
		}
		else
		{
			m_WorldPosition = m_LocalPosition;
		}
		m_DirtyWorldPosition = false;
	}
	return m_WorldPosition;
}

void dae::TransformComponent::SetDirtyWorldPosition()
{
	m_DirtyWorldPosition = true;
}

