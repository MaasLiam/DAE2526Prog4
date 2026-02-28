#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <assert.h>
#include <algorithm>
#include "TransformComponent.h"


void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_Components)
	{
		component->Update(deltaTime);
	}

	//remove components that were marked for removal
	if (!m_ComponentsToRemove.empty())
	{
		m_Components.erase(
			std::remove_if(
				m_Components.begin(),
				m_Components.end(),
				[&](const std::unique_ptr<Component>& ptr)
				{
					return std::find(m_ComponentsToRemove.begin(), m_ComponentsToRemove.end(), ptr.get()) != m_ComponentsToRemove.end();
				}
			),
			m_Components.end()
		);
		m_ComponentsToRemove.clear();
	}
}

void dae::GameObject::Render() const
{
	for (auto const& component : m_Components)
	{
		component->Render();
	}
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_pParent == parent)
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
	if (m_pParent) m_pParent->RemoveChild(this);
	m_pParent = parent;
	if (m_pParent) m_pParent->AddChild(this);

}

size_t dae::GameObject::GetChildCount() const
{
	return m_Children.size();
}

dae::GameObject* dae::GameObject::GetChildAt(size_t index) const
{
	if (index >= m_Children.size()) return nullptr;
	return m_Children[index];
}

const std::vector<dae::GameObject*>& dae::GameObject::GetChildren() const
{
	return m_Children;
}

void dae::GameObject::AddChild(GameObject* child)
{
	m_Children.push_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
}

bool dae::GameObject::IsChild(GameObject* gameObject)
{
	return std::find(m_Children.begin(), m_Children.end(), gameObject) != m_Children.end();
}

void dae::GameObject::SetLocalPosition(glm::vec3 pos)
{
	TransformComponent* transform = GetComponent<TransformComponent>();
	transform->SetLocalPosition(pos);
}

glm::vec3 dae::GameObject::GetWorldPos() const
{
	return GetComponent<TransformComponent>()->GetWorldPosition();
}

void dae::GameObject::SetDirtyWorldPosition()
{
	if (auto* t = GetComponent<TransformComponent>())
		t->SetDirtyWorldPosition();
}
