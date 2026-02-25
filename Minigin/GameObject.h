#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include <vector>
#include <glm/vec3.hpp>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		template <typename T, typename... Args>
		void AddComponent(Args&&... args)
		{

			m_Components.emplace_back(std::make_unique<T>(this, std::forward<Args>(args)...));
		}

		template<typename T>
		T* GetComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (auto casted = dynamic_cast<T*>(component.get()))
				{
					return casted;
				}
			}
			return nullptr;
		}

		template <typename T>
		bool HasComponent() const
		{
			return GetComponent<T>() != nullptr;
		}

		template<typename T>
		void RemoveComponent()
		{
			//mark for removal
			if (auto* component = GetComponent<T>())
			{
				m_ComponentsToRemove.push_back(component);
			}
		}

		GameObject* GetParent() const;
		void SetParent(GameObject* parent, bool keepWorldPosition);

		size_t GetChildCount() const;
		GameObject* GetChildAt(size_t index) const;
		const std::vector<GameObject*>& GetChildren() const;
		glm::vec3 GetWorldPos() const;
		void SetDirtyWorldPosition();
		
		
		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		

	private:
		std::vector<std::unique_ptr<Component>> m_Components{};
		std::vector<Component*> m_ComponentsToRemove{};

		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_Children{};

		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);
		bool IsChild(GameObject* gameObject);
		void SetLocalPosition(glm::vec3 pos);
		
		
		
	};
}
