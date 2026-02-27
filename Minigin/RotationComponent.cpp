#include "RotationComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include <numbers>


dae::RotationComponent::RotationComponent(GameObject* owner, float radius, float angularSpeed)
	: Component(owner)
	, m_Radius(radius)
	, m_AngularSpeed(angularSpeed)
	, m_pTransform(owner->GetComponent<TransformComponent>())
{

}

void dae::RotationComponent::Update(float deltaTime)
{
    if (!m_pTransform) return;

    m_Angle += m_AngularSpeed * deltaTime;

    constexpr float twoPi = std::numbers::pi_v<float> *2.f;

    if (m_Angle >= twoPi)
    {
        m_Angle -= twoPi;
    }
    else if (m_Angle < 0.f)
    {
        m_Angle += twoPi;
    }
        

    const glm::vec3 offset{std::cos(m_Angle) * m_Radius, std::sin(m_Angle) * m_Radius, 0.f };

    m_pTransform->SetLocalPosition(offset);
}
