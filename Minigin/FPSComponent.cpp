#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include <format>

dae::FPSComponent::FPSComponent(GameObject* parent)
	: Component(parent)
	, m_UpdateInterval(0.1f)
{
}

void dae::FPSComponent::Update(float deltaTime)
{
    if (!m_pTextComponent)
        m_pTextComponent = m_Parent->GetComponent<TextComponent>();

    if (!m_pTextComponent)
        return;

    if (deltaTime <= 0.f)
        return;

    m_AccumulatedTime += deltaTime;
    ++m_FrameCount;

    if (m_AccumulatedTime >= m_UpdateInterval)
    {
        const float fps = static_cast<float>(m_FrameCount) / m_AccumulatedTime;

        m_pTextComponent->SetText(std::format("{:.1f} FPS", fps));

        m_AccumulatedTime = 0.f;
        m_FrameCount = 0;
    }
}