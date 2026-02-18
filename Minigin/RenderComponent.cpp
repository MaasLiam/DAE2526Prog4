#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "TransformComponent.h"

dae::RenderComponent::RenderComponent(GameObject* parent, const std::string& fileName)
	: Component(parent)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void dae::RenderComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		auto* transform = m_Parent->GetComponent<TransformComponent>();
		const auto& pos = transform != nullptr ? transform->GetPosition() : glm::vec3{ 0, 0, 0 };

		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

