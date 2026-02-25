#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class RotationComponent final : public Component
	{
	public:
		RotationComponent(GameObject* owner, float radius, float angularSpeed, const glm::vec3& center = { 0.f,0.f,0.f });
		void Update(float deltaTime) override;
	private:
		float m_Radius{};
		float m_AngularSpeed{};
		float m_Angle{};
		glm::vec3 m_Center{};
	};

}