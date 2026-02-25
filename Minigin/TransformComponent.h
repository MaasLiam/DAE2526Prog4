#pragma once
#include <glm/glm.hpp>
#include "Component.h"


namespace dae
{
	class GameObject;
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* parent);
		void Update(float) override {};
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		void SetLocalPosition(float x, float y, float z = 0);
		void SetLocalPosition(const glm::vec3& position);
		const glm::vec3& GetWorldPosition() const;
		void SetDirtyWorldPosition();
	private:
		glm::vec3 m_LocalPosition{};
		mutable glm::vec3 m_WorldPosition{};
		mutable bool m_DirtyWorldPosition{true};
	};
}