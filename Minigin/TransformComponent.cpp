#include "TransformComponent.h"

#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* parent)
	: Component(parent)
{

}

const glm::vec3& dae::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec3& dae::TransformComponent::GetWorldPosition() const
{
	if (!m_IsWorldPositionDirty)
	{
		return m_WorldPosition;
	}

	const auto* parent = GetOwner()->GetParent();

	if (parent == nullptr)
	{
		m_WorldPosition = m_LocalPosition;
		m_IsWorldPositionDirty = false;
		return m_WorldPosition;
	}

	const auto* parentTransform = parent->GetComponent<TransformComponent>();

	if (parentTransform == nullptr)
	{
		m_WorldPosition = m_LocalPosition;
		m_IsWorldPositionDirty = false;
		return m_WorldPosition;
	}

	m_WorldPosition = parentTransform->GetWorldPosition() + m_LocalPosition;
	m_IsWorldPositionDirty = false;

	return m_WorldPosition;
}

void dae::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	SetLocalPosition(glm::vec3{ x, y, z });
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	if (m_LocalPosition == position)
	{
		return;
	}

	m_LocalPosition = position;
	SetDirtyWorldPosition();
}

void dae::TransformComponent::SetDirtyWorldPosition()
{
	if (m_IsWorldPositionDirty)
	{
		return;
	}

	m_IsWorldPositionDirty = true;

	for (size_t index{}; index < GetOwner()->GetChildCount(); ++index)
	{
		auto* child = GetOwner()->GetChildAt(index);

		if (child == nullptr)
		{
			continue;
		}

		if (auto* childTransform = child->GetComponent<TransformComponent>())
		{
			childTransform->SetDirtyWorldPosition();
		}
	}
}