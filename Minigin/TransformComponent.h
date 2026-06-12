#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

namespace dae
{
	class GameObject;

	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* parent);

		~TransformComponent() override = default;

		TransformComponent(const TransformComponent&) = delete;
		TransformComponent(TransformComponent&&) = delete;
		TransformComponent& operator=(const TransformComponent&) = delete;
		TransformComponent& operator=(TransformComponent&&) = delete;

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition() const;

		void SetLocalPosition(float x, float y, float z = 0.f);
		void SetLocalPosition(const glm::vec3& position);

		void SetDirtyWorldPosition();

	private:
		glm::vec3 m_LocalPosition{};
		mutable glm::vec3 m_WorldPosition{};
		mutable bool m_IsWorldPositionDirty{ true };
	};
}