#include "FPSComponent.h"

#include "GameObject.h"
#include "TextComponent.h"

#include <string>

dae::FPSComponent::FPSComponent(GameObject* parent)
	: Component(parent)
	, m_TextComponent(parent->GetComponent<TextComponent>())
{

}

void dae::FPSComponent::Update(float deltaTime)
{
	if (m_TextComponent == nullptr || deltaTime <= 0.f)
	{
		return;
	}

	m_AccumulatedTime += deltaTime;
	++m_FrameCount;

	if (m_AccumulatedTime < UpdateInterval)
	{
		return;
	}

	const float fps = static_cast<float>(m_FrameCount) / m_AccumulatedTime;
	m_TextComponent->SetText(std::to_string(static_cast<int>(fps)) + " FPS");

	m_AccumulatedTime = 0.f;
	m_FrameCount = 0;
}