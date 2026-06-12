#pragma once

#include "Component.h"

#include <algorithm>
#include <memory>
#include <vector>

#include <glm/vec3.hpp>

namespace dae
{
	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject() = default;

		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		void Update(float deltaTime);
		void Render() const;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			auto& componentReference = *component;

			m_Components.emplace_back(std::move(component));

			return componentReference;
		}

		template <typename T>
		T* GetComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (auto* castedComponent = dynamic_cast<T*>(component.get()))
				{
					return castedComponent;
				}
			}

			return nullptr;
		}

		template <typename T>
		bool HasComponent() const
		{
			return GetComponent<T>() != nullptr;
		}

		template <typename T>
		void RemoveComponent()
		{
			if (auto* component = GetComponent<T>())
			{
				const auto alreadyMarkedForRemoval = std::find(m_ComponentsToRemove.begin(), m_ComponentsToRemove.end(), component) != m_ComponentsToRemove.end();
				if (alreadyMarkedForRemoval)
				{
					return;
				}

				m_ComponentsToRemove.emplace_back(component);
			}
		}

		GameObject* GetParent() const;
		void SetParent(GameObject* parent, bool keepWorldPosition);

		size_t GetChildCount() const;
		GameObject* GetChildAt(size_t index) const;

		glm::vec3 GetWorldPos() const;
		void SetDirtyWorldPosition();

	private:
		std::unique_ptr<GameObject> AddChild(std::unique_ptr<GameObject> child);
		std::unique_ptr<GameObject> RemoveChild(const GameObject* child);

		bool IsChild(const GameObject* gameObject) const;
		void SetLocalPosition(const glm::vec3& position);

		std::vector<std::unique_ptr<Component>> m_Components{};
		std::vector<Component*> m_ComponentsToRemove{};

		GameObject* m_Parent{};
		std::vector<std::unique_ptr<GameObject>> m_Children{};
	};
}