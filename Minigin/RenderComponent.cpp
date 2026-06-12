#include "RenderComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TransformComponent.h"

dae::RenderComponent::RenderComponent(GameObject* parent)
	: Component(parent)
	, m_pTransform(parent->GetComponent<TransformComponent>())
{}

dae::RenderComponent::RenderComponent(GameObject* parent, const std::string& filename)
	: Component(parent)
	, m_pTransform(parent->GetComponent<TransformComponent>())
{
	const auto texture = ResourceManager::GetInstance().LoadTexture(filename);
	m_pTexture = texture.get();
}

void dae::RenderComponent::Render() const
{
	if (m_pTexture == nullptr || m_pTransform == nullptr)
	{
		return;
	}

	const auto& position = m_pTransform->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, position.x, position.y);
}

void dae::RenderComponent::SetTexture(Texture2D* texture)
{
	m_pTexture = texture;
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	const auto texture = ResourceManager::GetInstance().LoadTexture(filename);
	m_pTexture = texture.get();
}