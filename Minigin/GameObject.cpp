#include "GameObject.h"

#include "TransformComponent.h"

#include <algorithm>

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_Components)
	{
		component->Update(deltaTime);
	}

	if (m_ComponentsToRemove.empty())
	{
		return;
	}

	m_Components.erase(
		std::remove_if(
			m_Components.begin(),
			m_Components.end(),
			[this](const std::unique_ptr<Component>& component)
			{
				return std::find(
					m_ComponentsToRemove.begin(),
					m_ComponentsToRemove.end(),
					component.get()
				) != m_ComponentsToRemove.end();
			}
		),
		m_Components.end()
	);

	m_ComponentsToRemove.clear();
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component->Render();
	}
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_Parent;
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_Parent == parent)
	{
		return;
	}

	if (parent == nullptr)
	{
		SetLocalPosition(GetWorldPos());
	}
	else
	{
		if (keepWorldPosition)
		{
			SetLocalPosition(GetWorldPos() - parent->GetWorldPos());
		}

		SetDirtyWorldPosition();
	}

	if (m_Parent != nullptr)
	{
		auto child = m_Parent->RemoveChild(this);

		if (parent != nullptr)
		{
			parent->AddChild(std::move(child));
		}
	}

	m_Parent = parent;
}

size_t dae::GameObject::GetChildCount() const
{
	return m_Children.size();
}

dae::GameObject* dae::GameObject::GetChildAt(size_t index) const
{
	if (index >= m_Children.size())
	{
		return nullptr;
	}

	return m_Children[index].get();
}

std::unique_ptr<dae::GameObject> dae::GameObject::AddChild(std::unique_ptr<GameObject> child)
{
	m_Children.emplace_back(std::move(child));

	return nullptr;
}

std::unique_ptr<dae::GameObject> dae::GameObject::RemoveChild(const GameObject* child)
{
	const auto iterator = std::find_if(
		m_Children.begin(),
		m_Children.end(),
		[child](const std::unique_ptr<GameObject>& currentChild)
		{
			return currentChild.get() == child;
		}
	);

	if (iterator == m_Children.end())
	{
		return nullptr;
	}

	auto removedChild = std::move(*iterator);
	m_Children.erase(iterator);

	return removedChild;
}

bool dae::GameObject::IsChild(const GameObject* gameObject) const
{
	return std::find_if(
		m_Children.begin(),
		m_Children.end(),
		[gameObject](const std::unique_ptr<GameObject>& child)
		{
			return child.get() == gameObject;
		}
	) != m_Children.end();
}

void dae::GameObject::SetLocalPosition(const glm::vec3& position)
{
	if (auto* transform = GetComponent<TransformComponent>())
	{
		transform->SetLocalPosition(position);
	}
}

glm::vec3 dae::GameObject::GetWorldPos() const
{
	if (const auto* transform = GetComponent<TransformComponent>())
	{
		return transform->GetWorldPosition();
	}

	return {};
}

void dae::GameObject::SetDirtyWorldPosition()
{
	if (auto* transform = GetComponent<TransformComponent>())
	{
		transform->SetDirtyWorldPosition();
	}
}