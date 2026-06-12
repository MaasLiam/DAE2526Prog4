#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"

namespace dae
{

	class Scene final
	{
	public:
		Scene() = default;
		~Scene() = default;

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

		void Add(std::unique_ptr<GameObject> object);
		void Remove(const GameObject& object);
		void RemoveAll();

		const std::vector<std::unique_ptr<GameObject>>& GetObjects() const;

		void Update(float deltaTime);
		void LateUpdate();
		void Render() const;

	private:
		void AddPendingObjects();
		void RemovePendingObjects();

		std::vector<std::unique_ptr<GameObject>> m_Objects{};
		std::vector<std::unique_ptr<GameObject>> m_ObjectsToAdd{};
		std::vector<const GameObject*> m_ObjectsToRemove{};

		bool m_IsUpdating{};
	};
}