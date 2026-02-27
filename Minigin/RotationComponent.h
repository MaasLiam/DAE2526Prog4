#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class TransformComponent;
	class RotationComponent final : public Component
	{
	public:
		RotationComponent(GameObject* owner, float radius, float angularSpeed);
		void Update(float deltaTime) override;
	private:
		float m_Radius{};
		float m_AngularSpeed{};
		float m_Angle{};
		TransformComponent* m_pTransform{};
	};

}