#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <assert.h>
#include <algorithm>

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