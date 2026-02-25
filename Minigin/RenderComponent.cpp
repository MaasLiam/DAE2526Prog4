#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "TransformComponent.h"

dae::RenderComponent::RenderComponent(GameObject& parent)
	: Component(parent)
{
}

dae::RenderComponent::RenderComponent(GameObject& parent, const std::string& filename)
	: Component(parent)
{
	auto tex = ResourceManager::GetInstance().LoadTexture(filename);
	m_pTexture = tex.get();
}

void dae::RenderComponent::Render() const
{
    if (!m_pTexture) return;

    auto* transform = GetOwner().GetComponent<TransformComponent>();
    if (!transform) return;

    const auto& pos = transform->GetPosition();
    Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::RenderComponent::SetTexture(Texture2D* texture)
{
	m_pTexture = texture;
}

